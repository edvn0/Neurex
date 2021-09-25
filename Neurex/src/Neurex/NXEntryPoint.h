#pragma once

#ifdef NX_PT_WIN

extern Neurex::Application* Neurex::create_application();

int main(int argc, char** argv) 
{
	Neurex::Logger::init();
	NX_CORE_WARN("test.-");
	NX_WARN("tets2");
	auto app = Neurex::create_application();
	app->start();
	delete app;
}

#else
#error Only win supported.
#endif