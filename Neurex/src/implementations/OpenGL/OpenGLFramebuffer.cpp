#include "OpenGLFramebuffer.h"
#include "nxpch.h"

#include "engine/core/GPUDevice.h"
#include <glad/glad.h>

namespace Engine {

namespace Utils {

	static GLenum TextureTarget(bool multisampled)
	{
		return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	static void bind_texture(bool multisampled, uint32_t id) { glBindTexture(TextureTarget(multisampled), id); }

	static void create_textures(bool multisampled, uint32_t* outID, uint32_t count)
	{
		glGenTextures(count, outID);
		glBindTexture(TextureTarget(multisampled), *outID);
	}

	static void attach_color_texture(
		uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
	{
		bool multisampled = samples > 1;
		if (multisampled) {
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
		} else {
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
	}

	static void attach_depth_texture(
		uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
	{
		bool multisampled = samples > 1;
		if (multisampled) {
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		} else {
			glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
	}

	static inline bool is_depth_format(FramebufferTextureFormat format)
	{
		return format == FramebufferTextureFormat::DEPTH24STENCIL8;
	}

	static GLenum EngineFramebufferTextureFormatToGL(FramebufferTextureFormat format)
	{
		switch (format) {
		case FramebufferTextureFormat::RGBA8:
			return GL_RGBA8;
		case FramebufferTextureFormat::RED_INTEGER:
			return GL_RED_INTEGER;
		case FramebufferTextureFormat::None:
			NX_CORE_ASSERT(false, "");
			return 0;
		case FramebufferTextureFormat::DEPTH24STENCIL8:
			NX_CORE_ASSERT(false, "");
			return 0;
		}

		NX_CORE_ASSERT(false, "");
		return 0;
	}

}

OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec_)
	: spec(spec_)
{
	for (auto spec : spec.attachments.attachments) {
		if (Utils::is_depth_format(spec.texture_format))
			depth_attachment_spec = spec;
		else
			color_attachment_specification.emplace_back(spec);
	}

	invalidate();
};

OpenGLFramebuffer::~OpenGLFramebuffer()
{
	glDeleteFramebuffers(1, &renderer_id);
	glDeleteTextures(color_attachments.size(), color_attachments.data());
	glDeleteTextures(1, &depth_attachment);
};

void OpenGLFramebuffer::invalidate()
{
	if (renderer_id) {
		glDeleteFramebuffers(1, &renderer_id);
		glDeleteTextures(color_attachments.size(), color_attachments.data());
		glDeleteTextures(1, &depth_attachment);

		color_attachments.clear();
		depth_attachment = 0;
	}

	glGenFramebuffers(1, &renderer_id);
	glBindFramebuffer(GL_FRAMEBUFFER, renderer_id);
	bool multisample = spec.samples > 1;

	// Attachments
	if (color_attachment_specification.size()) {
		color_attachments.resize(color_attachment_specification.size());
		Utils::create_textures(multisample, color_attachments.data(), color_attachments.size());
		for (size_t i = 0; i < color_attachments.size(); i++) {
			Utils::bind_texture(multisample, color_attachments[i]);
			switch (color_attachment_specification[i].texture_format) {
			case FramebufferTextureFormat::RGBA8:
				Utils::attach_color_texture(
					color_attachments[i], spec.samples, GL_RGBA8, GL_RGBA, spec.width, spec.height, i);
				break;
			case FramebufferTextureFormat::RED_INTEGER:
				Utils::attach_color_texture(
					color_attachments[i], spec.samples, GL_R32I, GL_RED_INTEGER, spec.width, spec.height, i);
				break;
			default:
				break;
			}
		}
	}

	if (depth_attachment_spec.texture_format != FramebufferTextureFormat::None) {
		Utils::create_textures(multisample, &depth_attachment, 1);
		Utils::bind_texture(multisample, depth_attachment);
		switch (depth_attachment_spec.texture_format) {
		case FramebufferTextureFormat::DEPTH24STENCIL8:
			Utils::attach_depth_texture(depth_attachment, spec.samples, GL_DEPTH24_STENCIL8,
				GL_DEPTH_STENCIL_ATTACHMENT, spec.width, spec.height);
			break;
		default:
			break;
		}
	}

	if (color_attachments.size() > 1) {
		NX_CORE_ASSERT(color_attachments.size() <= 4, "");
		GLenum buffers[4]
			= { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(color_attachments.size(), buffers);
	} else if (color_attachments.empty()) {
		// Only depth-pass
		glDrawBuffer(GL_NONE);
	}

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
	auto [x, y] = GPUDevice::get_framebuffer_max_size();
	if (width <= 0 || width >= x || height <= 0 || height > y) {
		NX_CORE_WARN("Tried to resize framebuffers into {} by {}", width, height);
		return;
	}
	spec.width = width;
	spec.height = height;
	invalidate();
};

int OpenGLFramebuffer::read_pixel(uint32_t attachmentIndex, int x, int y)
{
	NX_CORE_ASSERT(attachmentIndex < color_attachments.size(), "");

	glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
	int pixelData;
	glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
	return pixelData;
}

void OpenGLFramebuffer::clear_attachment(uint32_t attachmentIndex, int value)
{
	NX_CORE_ASSERT(attachmentIndex < color_attachments.size(), "");

	auto& spec = color_attachment_specification[attachmentIndex];
	glClearTexImage(color_attachments[attachmentIndex], 0,
		Utils::EngineFramebufferTextureFormatToGL(spec.texture_format), GL_INT, &value);
}

} // namespace Engine
