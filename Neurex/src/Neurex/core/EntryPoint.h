#pragma once

#ifdef NX_PT_WIN

extern Neurex::Application* Neurex::create_application();

int main(int argc, char** argv)
{
	Neurex::Logger::init();
	NX_CORE_WARN("test-core");
	NX_WARN("test-client");
	auto app = Neurex::create_application();
	app->run();
	delete app;
}

#elif defined(NX_PT_OSX)
extern Neurex::Application* Neurex::create_application();

int main(int argc, char** argv)
{
	Neurex::Logger::init();

	NX_PROFILE_BEGIN_SESSION("Startup", "NeurexProfile-Startup.json");
	auto app = Neurex::create_application();
	NX_PROFILE_END_SESSION();
	NX_PROFILE_BEGIN_SESSION("Runtime", "NeurexProfile-Runtime.json");
	app->run();
	NX_PROFILE_END_SESSION();
	NX_PROFILE_BEGIN_SESSION("Destruction", "NeurexProfile-Destruction.json");
	delete app;
	NX_PROFILE_END_SESSION();
}
#else
#error Only win and osx supported.
#endif