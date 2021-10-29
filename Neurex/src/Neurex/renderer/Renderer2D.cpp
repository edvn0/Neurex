#include "Renderer2D.h"
#include "nxpch.h"

#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"

namespace Neurex {

struct QuadVertex {
	glm::vec4 color;
	glm::vec3 position;
	glm::vec2 tex_coord;
	float tex_id;
	float tiling;
};

struct RendererData {
	const uint32_t max_quads = 25000;
	const uint32_t max_vertices = max_quads * 4;
	const uint32_t max_indices = max_quads * 6;

	static constexpr uint32_t max_texture_slots = 16;

	ref<VertexArray> quad_vertex_array;
	ref<VertexBuffer> quad_vertex_buffer;
	ref<Shader> shader;
	ref<Texture2D> white_texture;

	uint32_t quad_index_count = 0;
	QuadVertex* quad_vertex_buffer_base = nullptr;
	QuadVertex* quad_vertex_ptr = nullptr;

	std::array<ref<Texture2D>, max_texture_slots> texture_slots;
	uint32_t texture_slot_index = 1; // 0 is the white texture.
};

static RendererData render_data;

void Renderer2D::init()
{
	render_data.quad_vertex_array = VertexArray::create();
	render_data.quad_vertex_buffer = VertexBuffer::create(render_data.max_vertices * sizeof(QuadVertex));

	// clang-format off
	BufferLayout layout = { 
		{ ShaderDataType::Float4, "a_Color" },
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float2, "a_TexCoord" },
		{ ShaderDataType::Float, "a_TextureIndex" },
		{ ShaderDataType::Float, "a_Tiling" } 
	};
	// clang-format on

	render_data.quad_vertex_buffer->set_layout(layout);
	render_data.quad_vertex_array->add_vertex_buffer(render_data.quad_vertex_buffer);

	render_data.quad_vertex_buffer_base = new QuadVertex[render_data.max_vertices];

	ref<IndexBuffer> quad_index_buffer = IndexBuffer::create(render_data.max_quads);
	render_data.quad_vertex_array->set_index_buffer(quad_index_buffer);

	render_data.white_texture = Texture2D::create(1, 1);
	uint32_t data = 0xffffffff;
	render_data.white_texture->set_data(&data, sizeof(uint32_t));

	render_data.shader = Shader::create("assets/shaders/renderer_2d_shader.glsl");
	render_data.texture_slots[0] = render_data.white_texture;
}

void Renderer2D::shut_down() { delete[] render_data.quad_vertex_buffer_base; }

void Renderer2D::begin_scene(const OrthographicCamera& cam)
{
	render_data.shader->upload_uniform("u_ViewProjection", cam.get_view_projection_matrix());

	render_data.quad_index_count = 0;
	render_data.quad_vertex_ptr = render_data.quad_vertex_buffer_base;

	render_data.texture_slot_index = 1;
}

void Renderer2D::draw_quad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& colour)
{
	draw_rotated_quad(pos, 0.0, size, colour);
}

void Renderer2D::draw_quad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& colour)
{
	draw_quad({ pos.x, pos.y, 0.0f }, size, colour);
}

void Renderer2D::draw_rotated_quad(
	const glm::vec3& pos, float rotation, const glm::vec2& size, const glm::vec4& colour)
{
	NX_PROFILE_FUNCTION();

	render_data.quad_vertex_ptr->position = { pos.x, pos.y, 0.0f };
	render_data.quad_vertex_ptr->color = colour;
	render_data.quad_vertex_ptr->tex_id = 0.0f;
	render_data.quad_vertex_ptr->tiling = 1.0f;
	render_data.quad_vertex_ptr->tex_coord = { 0.0f, 0.0f };
	render_data.quad_vertex_ptr++;

	render_data.quad_vertex_ptr->position = { pos.x + size.x, pos.y, 0.0f };
	render_data.quad_vertex_ptr->color = colour;
	render_data.quad_vertex_ptr->tex_id = 0.0f;
	render_data.quad_vertex_ptr->tiling = 1.0f;
	render_data.quad_vertex_ptr->tex_coord = { 1.0f, 0.0f };
	render_data.quad_vertex_ptr++;

	render_data.quad_vertex_ptr->position = { pos.x + size.x, pos.y + size.y, 0.0f };
	render_data.quad_vertex_ptr->color = colour;
	render_data.quad_vertex_ptr->tex_id = 0.0f;
	render_data.quad_vertex_ptr->tiling = 1.0f;
	render_data.quad_vertex_ptr->tex_coord = { 1.0f, 1.0f };
	render_data.quad_vertex_ptr++;

	render_data.quad_vertex_ptr->position = { pos.x, pos.y + size.y, 0.0f };
	render_data.quad_vertex_ptr->color = colour;
	render_data.quad_vertex_ptr->tex_id = 0.0f;
	render_data.quad_vertex_ptr->tiling = 1.0f;
	render_data.quad_vertex_ptr->tex_coord = { 0.0f, 1.0f };
	render_data.quad_vertex_ptr++;

	render_data.quad_index_count += 6;
}

void Renderer2D::draw_quad(
	const glm::vec3& pos, const glm::vec2& size, const ref<Texture2D>& texture, float tiling_factor)
{
	constexpr glm::vec4 color(1.0f);

	int tex_index = 0;

	for (uint32_t i = 1; i < render_data.texture_slot_index; i++) {
		if (*render_data.texture_slots[i] == *texture) {
			tex_index = i;
			break;
		}
	}

	if (tex_index == 0) {
		tex_index = render_data.texture_slot_index;
		render_data.texture_slots[render_data.texture_slot_index] = texture;
		render_data.texture_slot_index++;
	}

	render_data.quad_vertex_ptr->position = { pos.x, pos.y, 0.0f };
	render_data.quad_vertex_ptr->color = color;
	render_data.quad_vertex_ptr->tex_id = tex_index;
	render_data.quad_vertex_ptr->tiling = tiling_factor;
	render_data.quad_vertex_ptr->tex_coord = { 0.0f, 0.0f };
	render_data.quad_vertex_ptr++;

	render_data.quad_vertex_ptr->position = { pos.x + size.x, pos.y, 0.0f };
	render_data.quad_vertex_ptr->color = color;
	render_data.quad_vertex_ptr->tex_id = tex_index;
	render_data.quad_vertex_ptr->tiling = tiling_factor;
	render_data.quad_vertex_ptr->tex_coord = { 1.0f, 0.0f };
	render_data.quad_vertex_ptr++;

	render_data.quad_vertex_ptr->position = { pos.x + size.x, pos.y + size.y, 0.0f };
	render_data.quad_vertex_ptr->color = color;
	render_data.quad_vertex_ptr->tex_id = tex_index;
	render_data.quad_vertex_ptr->tiling = tiling_factor;
	render_data.quad_vertex_ptr->tex_coord = { 1.0f, 1.0f };
	render_data.quad_vertex_ptr++;

	render_data.quad_vertex_ptr->position = { pos.x, pos.y + size.y, 0.0f };
	render_data.quad_vertex_ptr->color = color;
	render_data.quad_vertex_ptr->tex_id = tex_index;
	render_data.quad_vertex_ptr->tiling = tiling_factor;
	render_data.quad_vertex_ptr->tex_coord = { 0.0f, 1.0f };
	render_data.quad_vertex_ptr++;

	render_data.quad_index_count += 6;
}

void Renderer2D::draw_quad(
	const glm::vec2& pos, const glm::vec2& size, const ref<Texture2D>& texture, float tiling_factor)
{
	draw_quad({ pos.x, pos.y, 0.0f }, size, texture, tiling_factor);
}

void Renderer2D::end_scene()
{
	NX_PROFILE_FUNCTION();
	flush();
}

void Renderer2D::flush()
{
	NX_PROFILE_FUNCTION();

	if (render_data.quad_index_count == 0) {
		return;
	}

	uint32_t data_size = ((uint8_t*)render_data.quad_vertex_ptr - (uint8_t*)render_data.quad_vertex_buffer_base);
	render_data.quad_vertex_buffer->set_data(render_data.quad_vertex_buffer_base, data_size);

	for (uint32_t i = 0; i < render_data.texture_slot_index; i++)
		render_data.texture_slots[i]->bind(i);

	render_data.shader->bind();
	RenderCommand::draw_indexed(render_data.quad_vertex_array, render_data.quad_index_count);
};
}