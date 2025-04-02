#include "ogpch.h"
#include "Orange/Renderer/Buffer.h"

#include "Orange/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Orange
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: OG_CORE_ASSERT(false, "��ǰ����֧����Ⱦ�ӿڣ�None!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(size);
		}

		OG_CORE_ASSERT(false, "δ֪����Ⱦ�ӿڣ�");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: OG_CORE_ASSERT(false, "��ǰ����֧����Ⱦ�ӿڣ�None!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		OG_CORE_ASSERT(false, "δ֪����Ⱦ�ӿڣ�");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: OG_CORE_ASSERT(false, "��ǰ����֧����Ⱦ�ӿڣ�None!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLIndexBuffer>(indices, size);
		}

		OG_CORE_ASSERT(false, "δ֪����Ⱦ�ӿڣ�");
		return nullptr;
	}

}