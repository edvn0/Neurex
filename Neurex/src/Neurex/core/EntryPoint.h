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
	NX_CORE_WARN("test-core");
	NX_WARN("test-client");
	auto app = Neurex::create_application();
	app->run();
	delete app;
}
#else
#error Only win and osx supported.
#endif