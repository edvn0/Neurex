#pragma once

#include <Neurex.h>

using namespace Neurex;

class Sandbox2D : public Layer {
public:
	Sandbox2D();
	~Sandbox2D() = default;

	void attached() override;
	void detached() override;
	void updated(Timestep ts) override;
	void on_event(Event& event) override;
	void on_imgui_render() override;

private:
	OrthographicCameraController camera_controller;

	ref<VertexArray> square_vertex_array;
	ref<VertexBuffer> square_vertex_buffer;
	ref<IndexBuffer> square_index_buffer;
	ref<Shader> flat_color_shader;
	ref<Texture2D> checkerboard_texture;

	glm::vec4 square_color = { 0.2f, 0.3f, 0.8f, 1.0f };
};