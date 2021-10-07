#pragma once

#include <glm/glm.hpp>
#include <string.h>

namespace Neurex {
class Shader {
public:
	virtual ~Shader() = default;
	virtual void bind() = 0;
	virtual void unbind() = 0;

	virtual void upload_uniform(const std::string& name, const glm::mat4& float4_matrix) = 0;
	virtual void upload_uniform(const std::string& name, const glm::vec4& float4_vec) = 0;
	virtual void upload_uniform(const std::string& name, const glm::vec3& float3_vec) = 0;
	virtual void upload_uniform(const std::string& name, const glm::vec2& float2_vec) = 0;
	virtual void upload_uniform(const std::string& name, float float_val) = 0;
	virtual void upload_uniform(const std::string& name, int int_val) = 0;

	static Shader* create(const std::string& vertex, const std::string& fragment);
};
};