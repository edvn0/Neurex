#pragma once

#include "RenderCommand.h"

namespace Neurex
{

	class Renderer {
	public:
		static void begin_scene();

		static void submit(const std::shared_ptr<VertexArray>& va);

		static void end_scene();

		inline static RendererAPI::API get_api() { return RendererAPI::get_api(); }
	};

}