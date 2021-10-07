#include "Renderer.h"
#include "nxpch.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Neurex {

Renderer::SceneData* Renderer::scene_data = new Renderer::SceneData();

void Renderer::begin_scene(OrthographicCamera& camera)
{
	scene_data->view_projection_matrix = camera.get_view_projection_matrix();
}

void Renderer::submit(const ref<VertexArray>& va, const ref<Shader>& shader, const glm::mat4& transform)
{
	shader->bind();
	shader->upload_uniform("uniform_transform", transform);
	shader->upload_uniform("uniform_view_projection", scene_data->view_projection_matrix);

	va->bind();
	RenderCommand::draw_indexed(va);
}

void Renderer::end_scene()
{
}

}