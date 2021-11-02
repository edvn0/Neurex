#include "UniformBuffer.h"
#include "nxpch.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace Neurex {

ref<UniformBuffer> UniformBuffer::create(uint32_t w, uint32_t h)
{
	switch (Renderer::get_api()) {
	case RendererAPI::API::None: {
		NX_CORE_ASSERT(false, "No Rendering API is not supported.");
		return nullptr;
	}
	case RendererAPI::API::OpenGL: {
		return make_ref<OpenGLUniformBuffer>(w, h);
	}
	case RendererAPI::API::Vulkan: {
		NX_CORE_ASSERT(false, "Vulkan is not supported.");
		return nullptr;
	}
	case RendererAPI::API::Metal: {
		NX_CORE_ASSERT(false, "Metal is not supported.");
		return nullptr;
	}
	case RendererAPI::API::DirectX: {
		NX_CORE_ASSERT(false, "DirectX is not supported.");
		return nullptr;
	}
	}
	return nullptr;
}

} // namespace Neurex
