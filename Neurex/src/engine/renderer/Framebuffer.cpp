#include "Framebuffer.h"
#include "nxpch.h"

#include "Renderer.h"

#include "implementations/OpenGL/OpenGLFramebuffer.h"

namespace Engine {

ref<Framebuffer> Framebuffer::create(const FramebufferSpecification& spec)
{
	switch (Renderer::get_api()) {
	case RendererAPI::API::None: {
		NX_CORE_ASSERT(false, "No Rendering API is not supported.");
		return nullptr;
	}
	case RendererAPI::API::OpenGL: {
		return make_ref<OpenGLFramebuffer>(spec);
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

} // namespace Engine
