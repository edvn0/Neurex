#pragma once
#include "Neurex/renderer/RendererAPI.h"

namespace Neurex {
class OpenGLRendererAPI : public RendererAPI {
public:
	void init() override;
	void clear() override;
	void set_clear_colour(const glm::vec4& color) override;
	void draw_indexed(const ref<VertexArray>& va) override;
	void draw_indexed(const ref<VertexArray>& va, uint32_t count) override;
};

}
