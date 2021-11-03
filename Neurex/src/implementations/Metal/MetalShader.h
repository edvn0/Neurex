#pragma once

#include "engine/renderer/Shader.h"

namespace Engine {

class MetalShader : public Shader {
public:
	MetalShader(const std::string& name, const std::string& vertex, const std::string& fragment);
	MetalShader(const std::string& path);
	~MetalShader() = default;

	virtual void bind() override;
	virtual void unbind() override;

	virtual void upload_uniform(const std::string& name, const glm::mat4& float4_matrix) override;
	virtual void upload_uniform(const std::string& name, const glm::vec4& float4_vec) override;
	virtual void upload_uniform(const std::string& name, const glm::vec3& float3_vec) override;
	virtual void upload_uniform(const std::string& name, const glm::vec2& float2_vec) override;
	virtual void upload_uniform(const std::string& name, float float_val) override;
	virtual void upload_uniform(const std::string& name, int int_val) override;
	virtual void upload_uniform(const std::string& name, int* vals, uint32_t count) override;

	virtual void set_mat4(const std::string& name, const glm::mat4& mat4) override;
	virtual void set_float4(const std::string& name, const glm::vec4& mat4) override;
	virtual void set_float3(const std::string& name, const glm::vec3& mat4) override;

	virtual const std::string& get_name() const override { return name; }

private:
	uint32_t renderer_id;
	std::string name;
};
}