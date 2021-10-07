#include "Texture.h"
#include "nxpch.h"

#include "Platform/Metal/MetalTexture.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Renderer.h"

namespace Neurex {

ref<Texture2D> Texture2D::create(const std::string& path)
{
	switch (Renderer::get_api()) {
	case RendererAPI::API::None: {
		NX_CORE_ASSERT(false, "No Rendering API is not supported.");
		return nullptr;
	}
	case RendererAPI::API::OpenGL: {
		return std::make_shared<OpenGLTexture2D>(path);
	}
	case RendererAPI::API::Vulkan: {
		NX_CORE_ASSERT(false, "Vulkan is not supported.");
		return nullptr;
	}
	case RendererAPI::API::Metal: {
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
