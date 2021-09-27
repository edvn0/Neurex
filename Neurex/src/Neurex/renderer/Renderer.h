#pragma once

namespace Neurex
{

	enum class RendererAPI
	{
		None = 0,
		OpenGL,
		DirectX,
		Vulkan,
		Metal
	};

	class Renderer {
	public:
		inline static RendererAPI get_api() { return renderer_api; }
	private:
		static RendererAPI renderer_api;

	};

}