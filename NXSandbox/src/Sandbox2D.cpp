#include "Sandbox2D.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox 2D")
	, camera_controller(16.0f / 9.0f)
{
}

void Sandbox2D::attached()
{
	RenderCommand::set_clear_colour({ 0.1f, 0.1f, 0.1f, 1 });
	checkerboard_texture = Texture2D::create("assets/textures/checkerboard.png");
	cherno_texture = Texture2D::create("assets/textures/cherno_logo.png");
	spritesheet = Texture2D::create("assets/sprites/PixelWeapons.png");

	weapon_zero = SpritesheetTexture::create(spritesheet, { 1, 2 }, { 32, 32 }, { 1, 2 });

	camera_controller.set_zoom(0.7f);
}

void Sandbox2D::detached() { }

void Sandbox2D::updated(Timestep ts)
{
	NX_PROFILE_FUNCTION();

	static constexpr auto radians = glm::radians(22.0f);

	{
		NX_PROFILE_SCOPE("CameraController::OnUpdate");
		camera_controller.on_update(ts);
	}

	RenderCommand::clear();
	{
		static float rotation_square = 20.0f;
		rotation_square += 30.0f * ts.get_seconds();
		if (rotation_square >= 360.0f)
			rotation_square = 0.0f;
		NX_PROFILE_SCOPE("Renderer2D::GPUCalls");
		Renderer2D::begin_scene(camera_controller.get_camera());
		Renderer2D::draw_rotated_quad({ -1.0f, 0.0f, -0.5f }, 0.0f, { 0.8f, 0.8f }, square_color);
		Renderer2D::draw_rotated_quad(
			{ -0.5f, -0.5f, -0.2f }, glm::radians(rotation_square), { 0.5f, 0.75f }, { 0.9f, 0.3f, 0.1f, 1.0f });
		Renderer2D::draw_rotated_quad(
			{ -3.0, -3.0, 0.2f }, glm::radians(-rotation_square), { 1.0f, 1.0f }, checkerboard_texture, 50.0f);
		Renderer2D::draw_rotated_quad({ 0.0f, 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f }, cherno_texture);

		Renderer2D::draw_rotated_sprite({ 2.0f, -1.5f, 0.0f }, 0.0f, { 1.0f, 2.0f }, weapon_zero);

		for (float x = -5.0f; x <= 5.0f; x += 0.5) {
			for (float y = -5.0f; y <= 5.0f; y += 0.5) {
				glm::vec4 col({ (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.5f });
				Renderer2D::draw_rotated_quad({ x, y, 0.0f }, 0.0f, { 0.249, 0.249 }, col);
			}
		}
		Renderer2D::end_scene();
	}
}

void Sandbox2D::on_imgui_render()
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

	std::string name = "None";

	auto stats = Renderer2D::get_stats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.draw_calls);
	ImGui::Text("Quads: %d", stats.quads_submitted);
	ImGui::Text("Vertices: %d", stats.quads_submitted * 4);
	ImGui::Text("Indices: %d", stats.quads_submitted * 6);

	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
	ImGui::Begin("Viewport");
	auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
	auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
	auto viewportOffset = ImGui::GetWindowPos();
	// m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
	// m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

	// m_ViewportFocused = ImGui::IsWindowFocused();
	// m_ViewportHovered = ImGui::IsWindowHovered();
	// Application::the().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	// m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

	// uint64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
	// ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1
	// },
	//	ImVec2{ 1, 0 });

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

void Sandbox2D::on_event(Event& event)
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