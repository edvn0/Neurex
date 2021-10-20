#include "RenderCommand.h"
#include "nxpch.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Neurex {

RendererAPI* RenderCommand::api = new OpenGLRendererAPI();

}