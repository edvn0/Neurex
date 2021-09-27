#pragma once

#include "Neurex/Core.h"
#include "Neurex/events/Event.h"

namespace Neurex {

	class NX_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void attached() {};
		virtual void detached() {};
		virtual void updated() {};
		virtual void on_event(Event& event) {};
		virtual void on_imgui_render() {};
		
		inline const std::string& get_name() const { return debug_name; };
	protected:
		std::string debug_name;
	};

}

