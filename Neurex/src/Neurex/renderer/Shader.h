#pragma once

#include <glm/glm.hpp>
#include <string.h>
#include <unordered_map>

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

	virtual void set_mat4(const std::string& name, const glm::mat4& mat4) = 0;
	virtual void set_float4(const std::string& name, const glm::vec4& mat4) = 0;
	virtual void set_float3(const std::string& name, const glm::vec3& mat4) = 0;
	virtual void set_int(const std::string& name, int val) { upload_uniform(name, val); };

	virtual const std::string& get_name() const = 0;

	static ref<Shader> create(const std::string& name, const std::string& vertex, const std::string& fragment);
	static ref<Shader> create(const std::string& path);
};

class ShaderLibrary {
public:
	void add(const ref<Shader>& shader);
	void add(const std::string& name, const ref<Shader>& shader);
	ref<Shader> load(const std::string& fp);
	ref<Shader> load(const std::string& name, const std::string& fp);

	ref<Shader> get(const std::string& name);

private:
	std::unordered_map<std::string, ref<Shader>> shaders;
};

};