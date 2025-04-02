#include "ogpch.h"
#include "Orange/Renderer/VertexArray.h"

#include "Orange/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Orange
{

	Ref<VertexArray> VertexArray::Create()
	{ 
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: OG_CORE_ASSERT(false, "��ǰ��Ⱦ�ӿڣ�None �ݲ�֧�֣�"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexArray>();
		}

		OG_CORE_ASSERT(false, "δ֪����Ⱦ���ӿڣ�");
		return nullptr;
	}

}