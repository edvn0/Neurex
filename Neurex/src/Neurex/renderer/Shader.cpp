#include "Shader.h"
#include "nxpch.h"

#include "Neurex/renderer/Renderer.h"

#include "Platform/Metal/MetalShader.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Neurex {

Shader* Shader::create(const std::string& vertex, const std::string& fragment)
{
	switch (Renderer::get_api()) {
	case RendererAPI::API::None: {
		NX_CORE_ASSERT(false, "No Rendering API is not supported.");
		return nullptr;
	}
	case RendererAPI::API::OpenGL: {
		return new OpenGLShader(vertex, fragment);
	}
	case RendererAPI::API::Vulkan: {
		NX_CORE_ASSERT(false, "Vulkan is not supported.");
		return nullptr;
	}
	case RendererAPI::API::Metal: {
		return new MetalShader(vertex, fragment);
	}
	case RendererAPI::API::DirectX: {
		NX_CORE_ASSERT(false, "DirectX is not supported.");
		return nullptr;
	}
	}
	return nullptr;
}
}
