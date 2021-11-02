#include "NeutraliseLayer.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

NeutraliseLayer::NeutraliseLayer()
	: Layer("Sandbox 2D")
	, camera_controller(16.0f / 9.0f)
{
}

void NeutraliseLayer::attached()
{
	RenderCommand::set_clear_colour({ 0.1f, 0.1f, 0.1f, 1 });
	checkerboard_texture = Texture2D::create("assets/textures/checkerboard.png");
	cherno_texture = Texture2D::create("assets/textures/cherno_logo.png");
	spritesheet = Texture2D::create("assets/sprites/PixelWeapons.png");

	weapon_zero = SpritesheetTexture::create(spritesheet, { 1, 2 }, { 32, 32 }, { 1, 2 });

	FramebufferSpecification spec;
	spec.width = 1280;
	spec.height = 720;
	framebuffer = Framebuffer::create(spec);

	editor_camera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

	// Entity
	auto square = active_scene->create_entity("Green Square");
	square.add_component<SpriteRendererComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });

	auto redSquare = active_scene->create_entity("Red Square");
	redSquare.add_component<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

	square_entity = square;

	camera_entity = active_scene->create_entity("Camera A");
	camera_entity.add_component<CameraComponent>();

	second_camera_entity = active_scene->create_entity("Camera B");
	auto& cc = second_camera_entity.add_component<CameraComponent>();
	cc.primary = false;

	class CameraController : public ScriptableEntity {
	public:
		virtual void on_create() override
		{
			auto& translation = get_component<TransformComponent>().translation;
			translation.x = rand() % 10 - 5.0f;
		}

		virtual void on_destroy() override { }

		virtual void on_update(Timestep ts) override
		{
			auto& translation = get_component<TransformComponent>().translation;

			float speed = 5.0f;

			if (Input::is_key_pressed(NX_KC_A))
				translation.x -= speed * ts.get_seconds();
			if (Input::is_key_pressed(NX_KC_D))
				translation.x += speed * ts.get_seconds();
			if (Input::is_key_pressed(NX_KC_W))
				translation.y += speed * ts.get_seconds();
			if (Input::is_key_pressed(NX_KC_S))
				translation.y -= speed * ts.get_seconds();
		}
	};

	camera_entity.add_component<NativeScriptComponent>().bind<CameraController>();
	second_camera_entity.add_component<NativeScriptComponent>().bind<CameraController>();
}

void NeutraliseLayer::detached() { }

void NeutraliseLayer::updated(Timestep ts)
{
	NX_PROFILE_FUNCTION();

	static constexpr auto radians = glm::radians(22.0f);

	{
		NX_PROFILE_SCOPE("CameraController::OnUpdate");
		camera_controller.on_update(ts);
	}

	{
		NX_PROFILE_SCOPE("Renderer2D::GPUCalls");
		framebuffer->bind();

		RenderCommand::clear();
		static float rotation_square = 20.0f;
		rotation_square += 30.0f * ts.get_seconds();
		if (rotation_square >= 360.0f)
			rotation_square = 0.0f;
		Renderer2D::begin_scene(camera_controller.get_camera());
		Renderer2D::draw_rotated_quad({ -1.0f, 0.0f, -0.5f }, 0.0f, { 0.8f, 0.8f }, square_color);
		Renderer2D::draw_rotated_quad(
			{ -0.5f, -0.5f, -0.2f }, glm::radians(rotation_square), { 0.5f, 0.75f }, { 0.9f, 0.3f, 0.1f, 1.0f });
		Renderer2D::draw_rotated_quad({ -3.0, -3.0, 0.2f }, glm::radians(-rotation_square), { 1.0f, 1.0f },
			checkerboard_texture, glm::vec4(1.0f), 50.0f);
		Renderer2D::draw_rotated_quad({ 0.0f, 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f }, cherno_texture);

		// Renderer2D::draw_rotated_sprite({ 2.0f, -1.5f, 0.0f }, 0.0f, { 1.0f, 2.0f }, weapon_zero);

		for (float x = -5.0f; x <= 5.0f; x += 0.5) {
			for (float y = -5.0f; y <= 5.0f; y += 0.5) {
				glm::vec4 col({ (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.5f });
				Renderer2D::draw_rotated_quad({ x, y, 0.0f }, 0.0f, { 0.249, 0.249 }, col);
			}
		}
		Renderer2D::end_scene();
		framebuffer->unbind();
	}
}

void NeutraliseLayer::on_imgui_render()
{

	// Note: Switch this to true to enable dockspace
	static bool dockspaceOpen = true;
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen) {
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the
	// pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 370.0f;
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	style.WindowMinSize.x = minWinSizeX;

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
			// ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);
			if (ImGui::MenuItem("Exit"))
				Application::the().exit();
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::Begin("Stats");

	auto stats = Renderer2D::get_stats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %lu", stats.draw_calls);
	ImGui::Text("Quads: %lu", stats.quads_submitted);
	ImGui::Text("Vertices: %lu", stats.quads_submitted * 4);
	ImGui::Text("Indices: %lu", stats.quads_submitted * 6);

	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("Viewport");
	auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
	auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
	auto viewportOffset = ImGui::GetWindowPos();
	viewport_bounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
	viewport_bounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

	viewport_focused = ImGui::IsWindowFocused();
	viewport_hovered = ImGui::IsWindowHovered();
	auto* imgui_layer = static_cast<ImGuiLayer*>(Application::the().get_imgui_layer().get());
	imgui_layer->block_events(!viewport_focused || !viewport_hovered);

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	if (viewportPanelSize.x != viewport_size.x || viewportPanelSize.y != viewport_size.y) {
		viewport_size = { viewportPanelSize.x, viewportPanelSize.y };
		framebuffer->resize(viewport_size.x, viewport_size.y);
		camera_controller.resize(viewport_size.x, viewport_size.y);
	}

	uint64_t textureID = framebuffer->get_color_attachment_id();
	ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ viewport_size.x, viewport_size.y }, ImVec2{ 0, 1 },
		ImVec2{ 1, 0 });

	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
			const wchar_t* path = (const wchar_t*)payload->Data;
		}
		ImGui::EndDragDropTarget();
	}

	ImGui::End();
	ImGui::PopStyleVar();

	ImGui::End();
}

void NeutraliseLayer::on_event(Event& event)
{
	camera_controller.on_event(event);
	EventDispatcher dispatcher(event);

	dispatcher.dispatch_event<KeyPressedEvent>([&](KeyPressedEvent& ev) {
		if (ev.get_key_code() == NX_KC_X) {
			return Application::the().exit();
		}
		return false;
	});
}