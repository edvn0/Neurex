#pragma once

#include "NXCore.h"

namespace Neurex {

	class NX_API Application
	{
	public:
		Application();
		virtual ~Application();

		void start();
	};

	Application* create_application();
}

