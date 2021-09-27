#include "nxpch.h"
#include "ImGuiLayer.h"

#include "Neurex/Application.h"

#include "imgui.h"
#include "Platform/OpenGL/ImGuiOpenGLRenderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Neurex {

	ImGuiLayer::ImGuiLayer() : Layer("ImGui")
	{

	}

	ImGuiLayer::~ImGuiLayer()
	{

	}

	void ImGuiLayer::attached()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_KeyPadEnter] = GLFW_KEY_KP_ENTER;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::detached()
	{

	}

	void ImGuiLayer::updated()
	{
		float time = (float)glfwGetTime();
		ImGuiIO& io = ImGui::GetIO();
		Window& app_window = Application::the().get_window();

		io.DisplaySize = ImVec2(app_window.get_width(), app_window.get_height());
		io.DeltaTime = current_time > 0.0 ? (time - current_time) : (1.0f / 60.0f);
		current_time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::on_event(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.dispatch_event<MouseButtonPressedEvent>([&](MouseButtonPressedEvent& e) {
			ImGuiIO& io = ImGui::GetIO();
			io.MouseDown[e.get_mouse_button()] = true;
			return false;
		});

		dispatcher.dispatch_event<MouseButtonReleasedEvent>([&](MouseButtonReleasedEvent& e) {
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
			glViewport(0, 0, e.get_width(), e.get_height());
			return false;
		});

		dispatcher.dispatch_event<KeyPressedEvent>([&](KeyPressedEvent& e) {
			ImGuiIO& io = ImGui::GetIO();
			io.KeysDown[e.get_key_code()] = true;
			io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
			io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
			io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
			io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
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

}