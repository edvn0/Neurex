#pragma once

#include <string.h>

namespace Neurex
{
	class Shader {
	public:
		Shader(const std::string& vertex, const std::string& fragment);
		~Shader();

		void bind();
		void unbind();
	private:
		uint32_t renderer_id;
	};
}