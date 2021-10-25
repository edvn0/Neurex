#include "ImGuiLayer.h"
#include "nxpch.h"

#include "Neurex/core/Application.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Neurex {

ImGuiLayer::ImGuiLayer()
	: Layer("ImGui")
{
}

ImGuiLayer::~ImGuiLayer() { }

void ImGuiLayer::attached()
{
	NX_PROFILE_FUNCTION();

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGui::StyleColorsClassic();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	auto* window = static_cast<GLFWwindow*>(
		Application::the().get_window().get_natively());
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiLayer::detached()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiLayer::on_event(Event& event)
{
	NX_PROFILE_FUNCTION();

	EventDispatcher dispatcher(event);

	dispatcher.dispatch_event<MouseButtonPressedEvent>(
		[&](MouseButtonPressedEvent& e) {
			ImGuiIO& io = ImGui::GetIO();
			io.MouseDown[e.get_mouse_button()] = true;
			return false;
		});

	dispatcher.dispatch_event<MouseButtonReleasedEvent>(
		[&](MouseButtonReleasedEvent& e) {
			ImGuiIO& io = ImGui::GetIO();
			io.MouseDown[e.get_mouse_button()] = false;
			return false;
		});

	dispatcher.dispatch_event<MouseMovedEvent>([&](MouseMovedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.get_x(), e.get_y());
		return false;
	});

	dispatcher.dispatch_event<MouseScrolledEvent>([&](MouseScrolledEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheel += e.get_offset_y();
		io.MouseWheelH += e.get_offset_x();
		return false;
	});

	dispatcher.dispatch_event<WindowResizeEvent>([&](WindowResizeEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.get_width(), e.get_height());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, (int)e.get_width(), (int)e.get_height());
		return false;
	});

	dispatcher.dispatch_event<WindowFramebufferEvent>(
		[&](WindowFramebufferEvent& e) {
			glViewport(0, 0, e.get_width(), e.get_height());
			return false;
		});

	dispatcher.dispatch_event<KeyPressedEvent>([&](KeyPressedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.get_key_code()] = true;
		io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL]
			|| io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT]
			|| io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
		io.KeyAlt
			= io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER]
			|| io.KeysDown[GLFW_KEY_RIGHT_SUPER];
		return false;
	});

	dispatcher.dispatch_event<KeyTypedEvent>([&](KeyTypedEvent& e) {
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.get_key_code();
		if (keycode > 0 && keycode < 0x10000) {
			io.AddInputCharacter((unsigned short)keycode);
		}

		return false;
	});
}

void ImGuiLayer::on_imgui_render()
{
	NX_PROFILE_FUNCTION();

	static bool show = true;
	ImGui::ShowDemoWindow(&show);
}

void ImGuiLayer::begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::end()
{
	NX_PROFILE_FUNCTION();

	ImGuiIO& io = ImGui::GetIO();
	Application& app = Application::the();
	Window& window = app.get_window();

	io.DisplaySize
		= ImVec2((float)window.get_width(), (float)window.get_height());

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		auto* backup_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_context);
	}
}

}