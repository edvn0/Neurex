#include "nxpch.h"
#include "Renderer.h"

namespace Neurex {
	
	Renderer::SceneData* Renderer::scene_data = new Renderer::SceneData();

	void Renderer::begin_scene(OrthographicCamera& camera)
	{
		scene_data->view_projection_matrix = camera.get_view_projection_matrix();
	}

	void Renderer::submit(const std::shared_ptr<VertexArray>& va, const std::shared_ptr<Shader>& shader)
	{
		shader->bind();
		shader->upload_uniform_mat4("uniform_view_projection", scene_data->view_projection_matrix);
		
		va->bind();
		RenderCommand::draw_indexed(va);
	}

	void Renderer::end_scene()
	{

	}

}