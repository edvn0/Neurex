#pragma once

#include "RendererAPI.h"

namespace Neurex {

class RenderCommand {
public:
	inline static void init()
	{
		api->init();
	}

	inline static void clear()
	{
		api->clear();
	};

	inline static void set_clear_colour(const glm::vec4& color)
	{
		api->set_clear_colour(color);
	}

	inline static void draw_indexed(const ref<VertexArray>& va)
	{
		api->draw_indexed(va);
	}

private:
	static RendererAPI* api;
};

}