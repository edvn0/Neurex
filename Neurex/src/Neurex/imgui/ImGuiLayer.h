#pragma once

#include "Neurex/Layer.h"

namespace Neurex {
    
    class NX_API ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

		void attached() override;
		void detached() override;
		void updated() override;
		void on_event(Event& event) override;
    private:


    private:
        float current_time = 0.0f;
    };

}

