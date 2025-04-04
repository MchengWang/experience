#include "EditorLayer.h"
#include "Orange/Scene/SceneSerializer.h"
#include "Orange/Utils/PlatformUtils.h"
#include "Orange/Math/Math.h"
#include "Orange/Scripting/ScriptEngine.h"
#include "Orange/Renderer/Font.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ImGuizmo.h"

namespace Orange {

	static Ref<Font> o_Font;

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), o_CameraController(1280.0f / 720.0f), o_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
	{
		o_Font = Font::GetDefault();
	}

	void EditorLayer::OnAttach()
	{
		HZ_PROFILE_FUNCTION();

		o_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");
		o_IconPlay = Texture2D::Create("Resources/Icons/PlayButton.png");
		o_IconPause = Texture2D::Create("Resources/Icons/PauseButton.png");
		o_IconSimulate = Texture2D::Create("Resources/Icons/SimulateButton.png");
		o_IconStep = Texture2D::Create("Resources/Icons/StepButton.png");
		o_IconStop = Texture2D::Create("Resources/Icons/StopButton.png");


		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGET, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		o_Framebuffer = Framebuffer::Create(fbSpec);

		o_EditorScene = CreateRef<Scene>();
		o_ActiveScene = o_EditorScene;

		auto commandLineArgs = Application::Get().GetSpecification().CommandLineArgs;
		if (commandLineArgs.Count > 1)
		{
			auto projectFilePath = commandLineArgs[1];
			OpenProject(projectFilePath);
		}
		else
		{
			// TODO(Yan): prompt the user to select a directory
			// NewProject();
			// If no project is opened, close Hazelnut
			// NOTE: this is while we don't have a new project path
			if (!OpenProject())
				Application::Get().Close();
		}

		o_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
		Renderer2D::SetLineWidth(4.0f);
	}

	void EditorLayer::OnDetach()
	{
		HZ_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		HZ_PROFILE_FUNCTION();

		o_ActiveScene->OnViewportResize((uint32_t)o_ViewportSize.x, (uint32_t)o_ViewportSize.y);

		// Resize
		if (FramebufferSpecification spec = o_Framebuffer->GetSpecification();
			o_ViewportSize.x > 0.0f && o_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != o_ViewportSize.x || spec.Height != o_ViewportSize.y))
		{
			o_Framebuffer->Resize((uint32_t)o_ViewportSize.x, (uint32_t)o_ViewportSize.y);
			o_CameraController.OnResize(o_ViewportSize.x, o_ViewportSize.y);
			o_EditorCamera.SetViewportSize(o_ViewportSize.x, o_ViewportSize.y);
		}

		// Render
		Renderer2D::ResetStats();
		o_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		// Clear our entity ID attachment to -1
		o_Framebuffer->ClearAttachment(1, -1);

		switch (o_SceneState)
		{
		case SceneState::Edit:
		{
			if (o_ViewportFocused)
				o_CameraController.OnUpdate(ts);

			o_EditorCamera.OnUpdate(ts);

			o_ActiveScene->OnUpdateEditor(ts, o_EditorCamera);
			break;
		}
		case SceneState::Simulate:
		{
			o_EditorCamera.OnUpdate(ts);

			o_ActiveScene->OnUpdateSimulation(ts, o_EditorCamera);
			break;
		}
		case SceneState::Play:
		{
			o_ActiveScene->OnUpdateRuntime(ts);
			break;
		}
		}

		auto [mx, my] = ImGui::GetMousePos();
		mx -= o_ViewportBounds[0].x;
		my -= o_ViewportBounds[0].y;
		glm::vec2 viewportSize = o_ViewportBounds[1] - o_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = o_Framebuffer->ReadPixel(1, mouseX, mouseY);
			o_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, o_ActiveScene.get());
		}

		OnOverlayRender();

		o_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		HZ_PROFILE_FUNCTION();

		// Note: Switch this to true to enable dockspace
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open Project...", "Ctrl+O"))
					OpenProject();

				ImGui::Separator();

				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
					SaveScene();

				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				ImGui::Separator();

				if (ImGui::MenuItem("Exit"))
					Application::Get().Close();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Script"))
			{
				if (ImGui::MenuItem("Reload assembly", "Ctrl+R"))
					ScriptEngine::ReloadAssembly();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		o_SceneHierarchyPanel.OnImGuiRender();
		o_ContentBrowserPanel->OnImGuiRender();

		ImGui::Begin("Stats");

#if 0
		std::string name = "None";
		if (o_HoveredEntity)
			name = o_HoveredEntity.GetComponent<TagComponent>().Tag;
		ImGui::Text("Hovered Entity: %s", name.c_str());
#endif

		auto stats = Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();

		ImGui::Begin("Settings");
		ImGui::Checkbox("Show physics colliders", &o_ShowPhysicsColliders);

		ImGui::Image((ImTextureID)o_Font->GetAtlasTexture()->GetRendererID(), { 512,512 }, { 0, 1 }, { 1, 0 });

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		o_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		o_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		o_ViewportFocused = ImGui::IsWindowFocused();
		o_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!o_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		o_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint64_t textureID = o_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ o_ViewportSize.x, o_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(path);
			}
			ImGui::EndDragDropTarget();
		}

		// Gizmos
		Entity selectedEntity = o_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && o_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(o_ViewportBounds[0].x, o_ViewportBounds[0].y, o_ViewportBounds[1].x - o_ViewportBounds[0].x, o_ViewportBounds[1].y - o_ViewportBounds[0].y);

			// Camera

			// Runtime camera from entity
			// auto cameraEntity = o_ActiveScene->GetPrimaryCameraEntity();
			// const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			// const glm::mat4& cameraProjection = camera.GetProjection();
			// glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

			// Editor camera
			const glm::mat4& cameraProjection = o_EditorCamera.GetProjection();
			glm::mat4 cameraView = o_EditorCamera.GetViewMatrix();

			// Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f; // Snap to 0.5m for translation/scale
			// Snap to 45 degrees for rotation
			if (o_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)o_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}


		ImGui::End();
		ImGui::PopStyleVar();

		UI_Toolbar();

		ImGui::End();
	}

	void EditorLayer::UI_Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		bool toolbarEnabled = (bool)o_ActiveScene;

		ImVec4 tintColor = ImVec4(1, 1, 1, 1);
		if (!toolbarEnabled)
			tintColor.w = 0.5f;

		float size = ImGui::GetWindowHeight() - 4.0f;
		ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (size * 0.5f));

		bool hasPlayButton = o_SceneState == SceneState::Edit || o_SceneState == SceneState::Play;
		bool hasSimulateButton = o_SceneState == SceneState::Edit || o_SceneState == SceneState::Simulate;
		bool hasPauseButton = o_SceneState != SceneState::Edit;

		if (hasPlayButton)
		{
			Ref<Texture2D> icon = (o_SceneState == SceneState::Edit || o_SceneState == SceneState::Simulate) ? o_IconPlay : o_IconStop;
			if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				if (o_SceneState == SceneState::Edit || o_SceneState == SceneState::Simulate)
					OnScenePlay();
				else if (o_SceneState == SceneState::Play)
					OnSceneStop();
			}
		}

		if (hasSimulateButton)
		{
			if (hasPlayButton)
				ImGui::SameLine();

			Ref<Texture2D> icon = (o_SceneState == SceneState::Edit || o_SceneState == SceneState::Play) ? o_IconSimulate : o_IconStop;
			if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
			{
				if (o_SceneState == SceneState::Edit || o_SceneState == SceneState::Play)
					OnSceneSimulate();
				else if (o_SceneState == SceneState::Simulate)
					OnSceneStop();
			}
		}
		if (hasPauseButton)
		{
			bool isPaused = o_ActiveScene->IsPaused();
			ImGui::SameLine();
			{
				Ref<Texture2D> icon = o_IconPause;
				if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
				{
					o_ActiveScene->SetPaused(!isPaused);
				}
			}

			// Step button
			if (isPaused)
			{
				ImGui::SameLine();
				{
					Ref<Texture2D> icon = o_IconStep;
					bool isPaused = o_ActiveScene->IsPaused();
					if (ImGui::ImageButton((ImTextureID)(uint64_t)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), tintColor) && toolbarEnabled)
					{
						o_ActiveScene->Step();
					}
				}
			}
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		o_CameraController.OnEvent(e);
		if (o_SceneState == SceneState::Edit)
		{
			o_EditorCamera.OnEvent(e);
		}

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(OG_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(OG_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		// Shortcuts
		if (e.IsRepeat())
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

		switch (e.GetKeyCode())
		{
			case Key::N:
			{
				if (control)
					NewScene();

				break;
			}
			case Key::O:
			{
				if (control)
					OpenProject();

				break;
			}
			case Key::S:
			{
				if (control)
				{
					if (shift)
						SaveSceneAs();
					else
						SaveScene();
				}

				break;
			}

			// Scene Commands
			case Key::D:
			{
				if (control)
					OnDuplicateEntity();

				break;
			}

			// Gizmos
			case Key::Q:
			{
				if (!ImGuizmo::IsUsing())
					o_GizmoType = -1;
				break;
			}
			case Key::W:
			{
				if (!ImGuizmo::IsUsing())
					o_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			}
			case Key::E:
			{
				if (!ImGuizmo::IsUsing())
					o_GizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			}
			case Key::R:
			{
				if (control)
				{
					ScriptEngine::ReloadAssembly();
				}
				else
				{
					if (!ImGuizmo::IsUsing())
						o_GizmoType = ImGuizmo::OPERATION::SCALE;
				}
				break;
			}
			case Key::Delete:
			{
				if (Application::Get().GetImGuiLayer()->GetActiveWidgetID() == 0)
				{
					Entity selectedEntity = o_SceneHierarchyPanel.GetSelectedEntity();
					if (selectedEntity)
					{
						o_SceneHierarchyPanel.SetSelectedEntity({});
						o_ActiveScene->DestroyEntity(selectedEntity);
					}
				}
				break;
			}
		}	

		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::ButtonLeft)
		{
			if (o_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
				o_SceneHierarchyPanel.SetSelectedEntity(o_HoveredEntity);
		}
		return false;
	}

	void EditorLayer::OnOverlayRender()
	{
		if (o_SceneState == SceneState::Play)
		{
			Entity camera = o_ActiveScene->GetPrimaryCameraEntity();
			if (!camera)
				return;

			Renderer2D::BeginScene(camera.GetComponent<CameraComponent>().Camera, camera.GetComponent<TransformComponent>().GetTransform());
		}
		else
		{
			Renderer2D::BeginScene(o_EditorCamera);
		}

		if (o_ShowPhysicsColliders)
		{
			// Box Colliders
			{
				auto view = o_ActiveScene->GetAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc, bc2d] = view.get<TransformComponent, BoxCollider2DComponent>(entity);

					glm::vec3 translation = tc.Translation + glm::vec3(bc2d.Offset, 0.001f);
					glm::vec3 scale = tc.Scale * glm::vec3(bc2d.Size * 2.0f, 1.0f);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), tc.Translation)
						* glm::rotate(glm::mat4(1.0f), tc.Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f))
						* glm::translate(glm::mat4(1.0f), glm::vec3(bc2d.Offset, 0.001f))
						* glm::scale(glm::mat4(1.0f), scale);

					Renderer2D::DrawRect(transform, glm::vec4(0, 1, 0, 1));
				}
			}

			// Circle Colliders
			{
				auto view = o_ActiveScene->GetAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();
				for (auto entity : view)
				{
					auto [tc, cc2d] = view.get<TransformComponent, CircleCollider2DComponent>(entity);

					glm::vec3 translation = tc.Translation + glm::vec3(cc2d.Offset, 0.001f);
					glm::vec3 scale = tc.Scale * glm::vec3(cc2d.Radius * 2.0f);

					glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation)
						* glm::scale(glm::mat4(1.0f), scale);

					Renderer2D::DrawCircle(transform, glm::vec4(0, 1, 0, 1), 0.01f);
				}
			}
		}

		// Draw selected entity outline 
		if (Entity selectedEntity = o_SceneHierarchyPanel.GetSelectedEntity())
		{
			const TransformComponent& transform = selectedEntity.GetComponent<TransformComponent>();
			Renderer2D::DrawRect(transform.GetTransform(), glm::vec4(1.0f, 0.5f, 0.0f, 1.0f));
		}

		Renderer2D::EndScene();
	}

	void EditorLayer::NewProject()
	{
		Project::New();
	}

	void EditorLayer::OpenProject(const std::filesystem::path& path)
	{
		if (Project::Load(path))
		{
			ScriptEngine::Init();

			auto startScenePath = Project::GetAssetFileSystemPath(Project::GetActive()->GetConfig().StartScene);
			OpenScene(startScenePath);
			o_ContentBrowserPanel = CreateScope<ContentBrowerPanel>();
		}
	}

	bool EditorLayer::OpenProject()
	{
		std::string filepath = FileDialogs::OpenFile("Orange Project (*.oproj)\0*.oproj\0");
		if (filepath.empty())
			return false;
		OpenProject(filepath);
		return true;
	}

	void EditorLayer::SaveProject()
	{
		// Project::SaveActive();
	}

	void EditorLayer::NewScene()
	{
		o_ActiveScene = CreateRef<Scene>();
		o_SceneHierarchyPanel.SetContext(o_ActiveScene);

		o_EditorScenePath = std::filesystem::path();
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Orange Scene (*.orange)\0*.orange\0");
		if (!filepath.empty())
			OpenScene(filepath);
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		if (o_SceneState != SceneState::Edit)
			OnSceneStop();

		if (path.extension().string() != ".orange")
		{
			OG_CLIENT_WARN("Could not load {0} - not a scene file", path.filename().string());
			return;
		}

		Ref<Scene> newScene = CreateRef<Scene>();
		SceneSerializer serializer(newScene);
		if (serializer.Deserialize(path.string()))
		{
			o_EditorScene = newScene;
			o_SceneHierarchyPanel.SetContext(o_EditorScene);

			o_ActiveScene = o_EditorScene;
			o_EditorScenePath = path;
		}
	}

	void EditorLayer::SaveScene()
	{
		if (!o_EditorScenePath.empty())
			SerializeScene(o_ActiveScene, o_EditorScenePath);
		else
			SaveSceneAs();
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Orange Scene (*.orange)\0*.orange\0");
		if (!filepath.empty())
		{
			SerializeScene(o_ActiveScene, filepath);
			o_EditorScenePath = filepath;
		}
	}

	void EditorLayer::SerializeScene(Ref<Scene> scene, const std::filesystem::path& path)
	{
		SceneSerializer serializer(scene);
		serializer.Serialize(path.string());
	}

	void EditorLayer::OnScenePlay()
	{
		if (o_SceneState == SceneState::Simulate)
			OnSceneStop();

		o_SceneState = SceneState::Play;

		o_ActiveScene = Scene::Copy(o_EditorScene);
		o_ActiveScene->OnRuntimeStart();

		o_SceneHierarchyPanel.SetContext(o_ActiveScene);
	}

	void EditorLayer::OnSceneSimulate()
	{
		if (o_SceneState == SceneState::Play)
			OnSceneStop();

		o_SceneState = SceneState::Simulate;

		o_ActiveScene = Scene::Copy(o_EditorScene);
		o_ActiveScene->OnSimulationStart();

		o_SceneHierarchyPanel.SetContext(o_ActiveScene);
	}

	void EditorLayer::OnSceneStop()
	{
		OG_CORE_ASSERT(o_SceneState == SceneState::Play || o_SceneState == SceneState::Simulate);

		if (o_SceneState == SceneState::Play)
			o_ActiveScene->OnRuntimeStop();
		else if (o_SceneState == SceneState::Simulate)
			o_ActiveScene->OnSimulationStop();

		o_SceneState = SceneState::Edit;

		o_ActiveScene = o_EditorScene;

		o_SceneHierarchyPanel.SetContext(o_ActiveScene);
	}

	void EditorLayer::OnScenePause()
	{
		if (o_SceneState == SceneState::Edit)
			return;

		o_ActiveScene->SetPaused(true);
	}

	void EditorLayer::OnDuplicateEntity()
	{
		if (o_SceneState != SceneState::Edit)
			return;

		Entity selectedEntity = o_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity)
		{
			Entity newEntity = o_EditorScene->DuplicateEntity(selectedEntity);
			o_SceneHierarchyPanel.SetSelectedEntity(newEntity);
		}
	}

}
