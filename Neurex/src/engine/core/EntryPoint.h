#pragma once

#ifdef NX_PT_WIN

extern Engine::Application* Engine::create_application();

int main(int argc, char** argv)
{
	Engine::Logger::init();
	NX_CORE_WARN("test-core");
	NX_WARN("test-client");
	auto app = Engine::create_application();
	app->run();
	delete app;
}

#elif defined(NX_PT_OSX)
extern Engine::Application* Engine::create_application();

int main(int argc, char** argv)
{
	Engine::Logger::init();

	NX_PROFILE_BEGIN_SESSION("Startup", "EngineProfile-Startup.json");
	auto app = Engine::create_application();
	NX_PROFILE_END_SESSION();
	NX_PROFILE_BEGIN_SESSION("Runtime", "EngineProfile-Runtime.json");
	app->run();
	NX_PROFILE_END_SESSION();
	NX_PROFILE_BEGIN_SESSION("Destruction", "EngineProfile-Destruction.json");
	delete app;
	NX_PROFILE_END_SESSION();

	Renderer2D::shut_down();
}
#else
#error Only win and osx supported.
#endif