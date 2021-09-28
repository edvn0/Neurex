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
		case RendererAPI::None:
		{
			NX_CORE_ASSERT(false, "No Rendering API is not supported.");
			return nullptr;
		}
		case RendererAPI::OpenGL:
		{
			return new OpenGLVertexArray();
		}
		case RendererAPI::Vulkan:
		{
			NX_CORE_ASSERT(false, "Vulkan is not supported.");
			return nullptr;
		}
		case RendererAPI::Metal:
		{
			NX_CORE_ASSERT(false, "Metal is not supported.");
			return nullptr;
		}
		case RendererAPI::DirectX:
		{
			NX_CORE_ASSERT(false, "DirectX is not supported.");
			return nullptr;
		}
		}
		return nullptr;
	}
}