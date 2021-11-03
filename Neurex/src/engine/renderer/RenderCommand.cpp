#include "RenderCommand.h"
#include "nxpch.h"

#include "implementations/OpenGL/OpenGLRendererAPI.h"

namespace Engine {

RendererAPI* RenderCommand::api = new OpenGLRendererAPI();

}