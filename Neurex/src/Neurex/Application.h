#pragma once

#include "Core.h"
#include "Neurex/events/ApplicationEvent.h"
#include "Neurex/events/KeyEvent.h"
#include "Neurex/events/MouseEvent.h"
#include "Neurex/Logger.h"

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

