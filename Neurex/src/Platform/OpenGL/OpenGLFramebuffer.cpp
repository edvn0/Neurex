#include "OpenGLFramebuffer.h"
#include "nxpch.h"

#include <glad/glad.h>

namespace Neurex {

OpenGLFramebuffer::~OpenGLFramebuffer() { glDeleteFramebuffers(1, &renderer_id); };

void OpenGLFramebuffer::invalidate()
{
	if (renderer_id) {
		glDeleteFramebuffers(1, &renderer_id);
		glDeleteTextures(1, &color_attachment);
		glDeleteTextures(1, &depth_attachment);
	}

	glGenFramebuffers(1, &renderer_id);
	glBindFramebuffer(GL_FRAMEBUFFER, renderer_id);

	glGenTextures(1, &color_attachment);
	glBindTexture(GL_TEXTURE_2D, color_attachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, spec.width, spec.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_attachment, 0);

	glGenTextures(1, &depth_attachment);
	glBindTexture(GL_TEXTURE_2D, depth_attachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, spec.width, spec.height, 0, GL_DEPTH_STENCIL,
		GL_UNSIGNED_INT_24_8, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depth_attachment, 0);

	NX_CORE_ASSERT(
		glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
};

void OpenGLFramebuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, renderer_id);
	glViewport(0, 0, spec.width, spec.height);
};

void OpenGLFramebuffer::unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); };

void OpenGLFramebuffer::resize(uint32_t width, uint32_t height)
{
	if (width <= 0 || width >= 8192 || height <= 0 || height > 8192) {
		NX_CORE_WARN("Tried to resize framebuffers into {} by {}", width, height);
		return;
	}
	spec.width = width;
	spec.height = height;
	invalidate();
};

} // namespace Neurex
