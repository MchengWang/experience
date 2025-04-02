#include "ogpch.h"
#include "Orange/Renderer/Texture.h"

#include "Orange/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Orange
{ 

	Ref<Texture2D> Texture2D::Create(const TextureSpecification& specification)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: OG_CORE_ASSERT(false, "��ǰ��Ⱦ�ӿڣ�None �ݲ�֧�֣�"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(specification);
		}

		OG_CORE_ASSERT(false, "δ֪����Ⱦ���ӿڣ�");
		return nullptr;
	}
	
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: OG_CORE_ASSERT(false, "��ǰ��Ⱦ�ӿڣ�None �ݲ�֧�֣�"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(path);
		}

		OG_CORE_ASSERT(false, "δ֪����Ⱦ���ӿڣ�");
		return nullptr;
	}

}
