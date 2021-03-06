#include "OpenGLRendererAPI.h"
#include "nxpch.h"

#include <glad/glad.h>

namespace Engine {

void OpenGLRendererAPI::init()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

	glDepthFunc(GL_LEQUAL);

	glEnable(GL_DEPTH_TEST);

	glCullFace(GL_BACK);
}

void OpenGLRendererAPI::clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

void OpenGLRendererAPI::set_clear_colour(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::draw_indexed(const ref<VertexArray>& va)
{
	auto count = va->get_index_buffer()->get_count();
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
};

void OpenGLRendererAPI::draw_indexed(const ref<VertexArray>& va, uint32_t count)
{
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
};
}
