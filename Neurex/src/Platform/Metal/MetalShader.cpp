#include "nxpch.h"

#include "MetalShader.h"

namespace Neurex {

MetalShader::MetalShader(const std::string& name, const std::string& vertex, const std::string& fragment){};

MetalShader::MetalShader(const std::string& path){};

void MetalShader::bind(){};

void MetalShader::unbind(){};

void MetalShader::upload_uniform(const std::string& name, const glm::mat4& uniform) { }

void MetalShader::upload_uniform(const std::string& name, const glm::vec4& uniform) { }
void MetalShader::upload_uniform(const std::string& name, const glm::vec3& uniform) { }

void MetalShader::upload_uniform(const std::string& name, const glm::vec2& uniform) { }

void MetalShader::upload_uniform(const std::string& name, float uniform) { }

void MetalShader::upload_uniform(const std::string& name, int* uniform, uint32_t count) { }

void MetalShader::upload_uniform(const std::string& name, int uniform) { }

void MetalShader::set_float4(const std::string& name, const glm::vec4& vec) { }

void MetalShader::set_float3(const std::string& name, const glm::vec3& vec) { }

void MetalShader::set_mat4(const std::string& name, const glm::mat4& mat4) { }
}
