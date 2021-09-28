#pragma once

#include <string.h>
#include <glm/glm.hpp>

namespace Neurex
{
	class Shader {
	public:
		Shader(const std::string& vertex, const std::string& fragment);
		~Shader();

		void bind();
		void unbind();

		void upload_uniform_mat4(const std::string& name, const glm::mat4& uniform);
	private:
		uint32_t renderer_id;
	};
}