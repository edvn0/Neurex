#include "nxpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Neurex {

	void OpenGLRendererAPI::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::set_clear_colour(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::draw_indexed(const std::shared_ptr<VertexArray>& va)
	{
		glDrawElements(GL_TRIANGLES, va->get_index_buffer()->get_count(), GL_UNSIGNED_INT, nullptr);
	}

}
