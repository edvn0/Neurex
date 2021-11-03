#pragma once

#include "engine/renderer/Framebuffer.h"

namespace Engine {

class OpenGLFramebuffer : public Framebuffer {
public:
	OpenGLFramebuffer(const FramebufferSpecification& spec);
	virtual ~OpenGLFramebuffer() override;

	void invalidate();

	virtual void bind() override;
	virtual void unbind() override;

	virtual void resize(uint32_t width, uint32_t height) override;

	virtual int read_pixel(uint32_t attachmentIndex, int x, int y) override;

	virtual void clear_attachment(uint32_t attachmentIndex, int value) override;

	virtual uint32_t get_color_attachment_id(uint32_t index = 0) override { return color_attachments[index]; }

	virtual const FramebufferSpecification& get_spec() const override { return spec; };

private:
	FramebufferSpecification spec;
	uint32_t renderer_id;

	std::vector<FramebufferTextureSpecification> color_attachment_specification;
	FramebufferTextureSpecification depth_attachment_spec = FramebufferTextureFormat::None;

	std::vector<uint32_t> color_attachments;
	uint32_t depth_attachment;
};

} // namespace Engine
