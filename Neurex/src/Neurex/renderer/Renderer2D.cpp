#include "Renderer2D.h"
#include "nxpch.h"

#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"
namespace Neurex {

static constexpr glm::vec4 white_color(1.0f);

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

	RendererStats stats;

	static constexpr glm::vec4 vertex_positions[4] = { { -0.5f, -0.5f, 0.0f, 1.0f }, { 0.5f, -0.5f, 0.0f, 1.0f },
		{ 0.5f, 0.5f, 0.0f, 1.0f }, { -0.5f, 0.5f, 0.0f, 1.0f } };

	static constexpr glm::vec2 vertex_coords[4]
		= { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

	std::array<ref<Texture2D>, max_texture_slots> texture_slots;
	uint32_t texture_slot_index = 1; // 0 is the white texture.
};

static RendererData render_data;

void Renderer2D::start_batch()
{
	render_data.quad_index_count = 0;
	render_data.quad_vertex_ptr = render_data.quad_vertex_buffer_base;
	render_data.texture_slot_index = 1;
};

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
	render_data.shader->bind();
	int32_t vals[render_data.max_texture_slots] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	render_data.shader->set_int_array("u_Textures", vals, render_data.max_texture_slots);

	render_data.texture_slots[0] = render_data.white_texture;
}

void Renderer2D::shut_down() { delete[] render_data.quad_vertex_buffer_base; }

void Renderer2D::begin_scene(const OrthographicCamera& cam)
{
	render_data.shader->upload_uniform("u_ViewProjection", cam.get_view_projection_matrix());

	render_data.stats.draw_calls = 0;
	render_data.stats.quads_submitted = 0;

	start_batch();
}

void Renderer2D::next_batch()
{
	flush();
	start_batch();
}

void Renderer2D::draw_rotated_quad(
	const glm::vec3& pos, float rotation_radians, const glm::vec2& size, const glm::vec4& colour)
{
	NX_PROFILE_FUNCTION();

	if (render_data.quad_index_count >= render_data.max_quads) {
		next_batch();
	}

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, pos);
	transform = glm::rotate(transform, rotation_radians, { 0, 0, 1.0f });
	transform = glm::scale(transform, { size.x, size.y, 1.0f });

	for (int i = 0; i < 4; i++) {
		QuadVertex quad_vertex = {
			.color = colour,
			.position = transform * render_data.vertex_positions[i],
			.tex_coord = render_data.vertex_coords[i],
			.tex_id = 0.0f,
			.tiling = 1.0f,
		};

		*render_data.quad_vertex_ptr = quad_vertex;
		render_data.quad_vertex_ptr++;
	}

	render_data.quad_index_count += 6;
	render_data.stats.quads_submitted++;
}

void Renderer2D::draw_rotated_quad(const glm::vec3& pos, float rotation_radians, const glm::vec2& size,
	const ref<Texture2D>& texture, float tiling_factor)
{
	if (render_data.quad_index_count >= render_data.max_quads) {
		next_batch();
	}

	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, pos);
	transform = glm::rotate(transform, rotation_radians, { 0, 0, 1.0f });
	transform = glm::scale(transform, { size.x, size.y, 1.0f });

	uint32_t tex_index = 0;

	for (uint32_t i = 1; i < render_data.texture_slot_index; i++) {
		if (*render_data.texture_slots[i] == *texture) {
			tex_index = i;
			break;
		}
	}

	if (tex_index == 0) { // did not find this texture cached
		tex_index = render_data.texture_slot_index;

		if (tex_index >= render_data.max_texture_slots) {
			next_batch();
		}

		render_data.texture_slots[render_data.texture_slot_index] = texture;
		render_data.texture_slot_index++;
	}

	for (int i = 0; i < 4; i++) {
		QuadVertex quad_vertex = {
			.color = white_color,
			.position = transform * render_data.vertex_positions[i],
			.tex_coord = render_data.vertex_coords[i],
			.tex_id = (float)tex_index,
			.tiling = tiling_factor,
		};

		*render_data.quad_vertex_ptr = quad_vertex;
		render_data.quad_vertex_ptr++;
	}

	render_data.quad_index_count += 6;
	render_data.stats.quads_submitted++;
}

void Renderer2D::draw_rotated_sprite(const glm::vec3& pos, float rotation_radians, const glm::vec2& size,
	const ref<SpritesheetTexture>& subtexture, float tiling_factor)
{
	if (render_data.quad_index_count >= render_data.max_quads) {
		next_batch();
	}

	auto tex_coords = subtexture->get_texture_coordinates();

	auto transform = glm::mat4(1.0f);
	transform = glm::translate(transform, pos);
	transform = glm::rotate(transform, rotation_radians, { 0, 0, 1.0f });
	transform = glm::scale(transform, { size.x, size.y, 1.0f });

	uint32_t tex_index = 0;
	for (uint32_t i = 1; i < render_data.texture_slot_index; i++) {
		if (*render_data.texture_slots[i] == *subtexture->get_base_texture()) {
			tex_index = i;
			break;
		}
	}

	if (tex_index == 0) { // did not find this texture cached
		tex_index = render_data.texture_slot_index;

		if (tex_index >= render_data.max_texture_slots) {
			next_batch();
		}

		render_data.texture_slots[render_data.texture_slot_index] = subtexture->get_base_texture();
		render_data.texture_slot_index++;
	}

	for (int i = 0; i < 4; i++) {
		QuadVertex quad_vertex = {
			.color = white_color,
			.position = transform * render_data.vertex_positions[i],
			.tex_coord = tex_coords[i],
			.tex_id = (float)tex_index,
			.tiling = tiling_factor,
		};

		*render_data.quad_vertex_ptr = quad_vertex;
		render_data.quad_vertex_ptr++;
	}

	render_data.quad_index_count += 6;
	render_data.stats.quads_submitted++;
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

	uint32_t data_size
		= (uint32_t)((uint8_t*)render_data.quad_vertex_ptr - (uint8_t*)render_data.quad_vertex_buffer_base);
	render_data.quad_vertex_buffer->set_data(render_data.quad_vertex_buffer_base, data_size);

	for (auto i = 0; i < render_data.texture_slot_index; i++) {
		render_data.texture_slots[i]->bind(i);
	}

	render_data.shader->bind();
	RenderCommand::draw_indexed(render_data.quad_vertex_array, render_data.quad_index_count);
	render_data.stats.draw_calls++;
};

RendererStats& Renderer2D::get_stats() { return render_data.stats; };

}