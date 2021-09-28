#include "nxpch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Neurex
{
	VertexArray* VertexArray::create()
	{
		switch (Renderer::get_api())
		{
		case RendererAPI::API::None:
		{
			NX_CORE_ASSERT(false, "No Rendering API is not supported.");
			return nullptr;
		}
		case RendererAPI::API::OpenGL:
		{
			return new OpenGLVertexArray();
		}
		case RendererAPI::API::Vulkan:
		{
			NX_CORE_ASSERT(false, "Vulkan is not supported.");
			return nullptr;
		}
		case RendererAPI::API::Metal:
		{
			NX_CORE_ASSERT(false, "Metal is not supported.");
			return nullptr;
		}
		case RendererAPI::API::DirectX:
		{
			NX_CORE_ASSERT(false, "DirectX is not supported.");
			return nullptr;
		}
		}
		return nullptr;
	}
}