#pragma once
#include "Neurex/renderer/RendererAPI.h"


namespace Neurex
{
    class OpenGLRendererAPI :
        public RendererAPI
    {
    public:
        void clear() override;
        void set_clear_colour(const glm::vec4& color) override;
        void draw_indexed(const std::shared_ptr<VertexArray>& va) override;

    };

}