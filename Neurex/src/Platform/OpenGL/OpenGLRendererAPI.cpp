#include "OpenGLRendererAPI.h"
#include "nxpch.h"

#include <glad/glad.h>

namespace Neurex {

void OpenGLRendererAPI::init()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

void OpenGLRendererAPI::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::set_clear_colour(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::draw_indexed(const ref<VertexArray>& va)
{
	auto count = va->get_index_buffer()->get_count();
	NX_CORE_INFO("{0}", count);
	glDrawElements(GL_TRIANGLES, va->get_index_buffer()->get_count(), GL_UNSIGNED_INT, nullptr);
};

}
