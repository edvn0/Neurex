#include "nxpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Neurex
{

	RendererAPI* RenderCommand::api = new OpenGLRendererAPI();

}