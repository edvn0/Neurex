#pragma once

#include "Neurex/renderer/Shader.h"

namespace Neurex {

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

	virtual const std::string& get_name() const override { return name; }

private:
	uint32_t renderer_id;
	std::string name;
};
}