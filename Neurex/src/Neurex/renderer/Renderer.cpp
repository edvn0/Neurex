#include "Renderer.h"
#include "nxpch.h"

#include "Platform/OpenGL/OpenGLShader.h"

#include "Renderer2D.h"

namespace Neurex {

Renderer::SceneData* Renderer::scene_data = new Renderer::SceneData();
bool Renderer::has_begun = false;
bool Renderer::has_ended = false;

void Renderer::init()
{
	RenderCommand::init();
	Renderer2D::init();
}

void Renderer::begin_scene(OrthographicCamera& camera)
{
	scene_data->view_projection_matrix = camera.get_view_projection_matrix();
	has_begun = true;
}

void Renderer::submit(const ref<VertexArray>& va, const ref<Shader>& shader,
	const glm::mat4& transform)
{
	NX_CORE_ASSERT(has_begun,
		"Submit must be called between Renderer::begin_scene and "
		"Renderer::end_scene.");
	shader->bind();
	shader->upload_uniform("u_Transform", transform);
	shader->upload_uniform(
		"u_ViewProjection", scene_data->view_projection_matrix);

	va->bind();
	RenderCommand::draw_indexed(va);
}

void Renderer::end_scene() { has_begun = false; }
}