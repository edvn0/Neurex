#include "Shader.h"
#include "nxpch.h"

#include "Neurex/renderer/Renderer.h"

#include "Platform/Metal/MetalShader.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Neurex {

ref<Shader> Shader::create(const std::string& name, const std::string& vertex,
	const std::string& fragment)
{
	switch (Renderer::get_api()) {
	case RendererAPI::API::None: {
		NX_CORE_ASSERT(false, "No Rendering API is not supported.");
		return nullptr;
	}
	case RendererAPI::API::OpenGL: {
		return make_ref<OpenGLShader>(name, vertex, fragment);
	}
	case RendererAPI::API::Vulkan: {
		NX_CORE_ASSERT(false, "Vulkan is not supported.");
		return nullptr;
	}
	case RendererAPI::API::Metal: {
		return make_ref<MetalShader>(name, vertex, fragment);
	}
	case RendererAPI::API::DirectX: {
		NX_CORE_ASSERT(false, "DirectX is not supported.");
		return nullptr;
	}
	}
	return nullptr;
}

ref<Shader> Shader::create(const std::string& path)
{
	switch (Renderer::get_api()) {
	case RendererAPI::API::None: {
		NX_CORE_ASSERT(false, "No Rendering API is not supported.");
		return nullptr;
	}
	case RendererAPI::API::OpenGL: {
		return make_ref<OpenGLShader>(path);
	}
	case RendererAPI::API::Vulkan: {
		NX_CORE_ASSERT(false, "Vulkan is not supported.");
		return nullptr;
	}
	case RendererAPI::API::Metal: {
		return make_ref<MetalShader>(path);
	}
	case RendererAPI::API::DirectX: {
		NX_CORE_ASSERT(false, "DirectX is not supported.");
		return nullptr;
	}
	}
	return nullptr;
}

void ShaderLibrary::add(const std::string& name, const ref<Shader>& shader)
{
	NX_CORE_ASSERT(shaders.find(name) == shaders.end(),
		"Shader with given name already exists.");
	shaders[name] = shader;
};

void ShaderLibrary::add(const ref<Shader>& shader)
{
	auto& name = shader->get_name();
	NX_CORE_ASSERT(shaders.find(name) == shaders.end(),
		"Shader with given name already exists.");
	add(name, shader);
};

ref<Shader> ShaderLibrary::load(const std::string& name, const std::string& fp)
{
	auto shader = Shader::create(fp);
	add(name, shader);
	return shader;
};

ref<Shader> ShaderLibrary::load(const std::string& fp)
{
	auto shader = Shader::create(fp);
	add(shader);
	return shader;
};

ref<Shader> ShaderLibrary::get(const std::string& name)
{
	NX_CORE_ASSERT(shaders.find(name) != shaders.end(),
		"You provided an incorrect id for a shader.");
	return shaders[name];
};
}