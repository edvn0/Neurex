#include "Renderer2D.h"
#include "nxpch.h"

#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Neurex {

struct RendererData {
	ref<VertexArray> quad_vertex_array;
	ref<Shader> flat_colour_shader;
	ref<Shader> texture_shader;
};

static RendererData* render_data;

void Renderer2D::init()
{
	render_data = new RendererData();
	render_data->quad_vertex_array = VertexArray::create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 0.0f, 1.0f
	};

	ref<VertexBuffer> square_vb = VertexBuffer::create(squareVertices, sizeof(squareVertices));
	square_vb->set_layout({ { ShaderDataType::Float3, "a_Position" }, { ShaderDataType::Float2, "a_TexCoord" } });
	render_data->quad_vertex_array->add_vertex_buffer(square_vb);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	ref<IndexBuffer> square_ib = IndexBuffer::create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	render_data->quad_vertex_array->set_index_buffer(square_ib);

	render_data->flat_colour_shader = Shader::create("assets/shaders/flat_colour.glsl");
	render_data->texture_shader = Shader::create("assets/shaders/texture.glsl");
	render_data->texture_shader->set_int("u_Texture", 0);
}

void Renderer2D::shut_down()
{
	delete render_data;
}

void Renderer2D::begin_scene(const OrthographicCamera& cam)
{
	auto& shader = *render_data->flat_colour_shader;
	shader.bind();
	shader.upload_uniform("u_ViewProjection", cam.get_view_projection_matrix());

	auto& texture_shader = *render_data->texture_shader;
	texture_shader.bind();
	texture_shader.upload_uniform("u_ViewProjection", cam.get_view_projection_matrix());
}

void Renderer2D::draw_quad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& colour)
{
	draw_rotated_quad(pos, { 0, 0, 0 }, size, colour);
}

void Renderer2D::draw_quad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& colour)
{
	draw_quad({ pos.x, pos.y, 0.0f }, size, colour);
}

void Renderer2D::draw_rotated_quad(const glm::vec3& pos, const glm::vec3& rot, const glm::vec2& size, const glm::vec4& colour)
{
	auto& shader = *render_data->flat_colour_shader;
	shader.bind();
	shader.set_float4("u_Color", colour);
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * glm::rotate(glm::mat4(1.0f), glm::radians(rot.x), { 1.0f, 0.0, 0 }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
	shader.set_mat4("u_Transform", transform);
	render_data->quad_vertex_array->bind();
	RenderCommand::draw_indexed(render_data->quad_vertex_array);
}

void Renderer2D::draw_quad(const glm::vec3& pos, const glm::vec2& size, const ref<Texture2D>& texture)
{
	auto& shader = *render_data->texture_shader;
	shader.bind();

	glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
	shader.set_mat4("u_Transform", transform);

	texture->bind();

	render_data->quad_vertex_array->bind();
	RenderCommand::draw_indexed(render_data->quad_vertex_array);
}

void Renderer2D::draw_quad(const glm::vec2& pos, const glm::vec2& size, const ref<Texture2D>& texture)
{
	draw_quad({ pos.x, pos.y, 0.0f }, size, texture);
}

void Renderer2D::end_scene()
{
}
}