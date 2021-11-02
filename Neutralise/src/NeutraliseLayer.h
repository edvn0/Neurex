#pragma once

#include <Neurex.h>

using namespace Neurex;

class NeutraliseLayer : public Layer {
public:
	NeutraliseLayer();
	~NeutraliseLayer() = default;

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
	ref<Texture2D> cherno_texture;
	ref<Texture2D> spritesheet;
	ref<SpritesheetTexture> weapon_zero;
	ref<Framebuffer> framebuffer;

	ref<Scene> active_scene;
	Entity square_entity;
	Entity camera_entity;
	Entity second_camera_entity;
	EditorCamera editor_camera;

	glm::vec2 viewport_size = { 0.0f, 0.0f };
	glm::vec2 viewport_bounds[2] = { { 0.0f, 0.0f }, { 0.0f, 0.0f } };
	bool viewport_focused = false, viewport_hovered = false;

	glm::vec4 square_color = { 0.2f, 0.3f, 0.8f, 0.8f };
};