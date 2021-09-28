#include "nxpch.h"
#include "Renderer.h"

namespace Neurex {
	
	void Renderer::begin_scene()
	{

	}

	void Renderer::submit(const std::shared_ptr<VertexArray>& va)
	{
		va->bind();
		RenderCommand::draw_indexed(va);
	}

	void Renderer::end_scene()
	{

	}

}