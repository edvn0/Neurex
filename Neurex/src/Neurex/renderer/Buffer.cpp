#include "nxpch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Neurex
{

	VertexBuffer* VertexBuffer::create(float* vertices, uint32_t size)
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
			return new OpenGLVertexBuffer(vertices, size);
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

	IndexBuffer* IndexBuffer::create(uint32_t* indices, uint32_t count)
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
			return new OpenGLIndexBuffer(indices, count);
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