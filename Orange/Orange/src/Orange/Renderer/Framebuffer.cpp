#include "ogpch.h"
#include "Orange/Renderer/Framebuffer.h"

#include "Orange/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Orange
{

	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: OG_CORE_ASSERT(false, "��ǰ����֧����Ⱦ�ӿڣ�None!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLFramebuffer>(spec);
		}

		OG_CORE_ASSERT(false, "δ֪����Ⱦ�ӿڣ�");
		return nullptr;
	}

}
