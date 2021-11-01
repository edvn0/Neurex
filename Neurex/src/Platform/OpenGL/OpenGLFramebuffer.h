#pragma once

#include "Neurex/renderer/Framebuffer.h"

namespace Neurex {

class OpenGLFramebuffer : public Framebuffer {
public:
	OpenGLFramebuffer(const FramebufferSpecification& spec_)
		: spec(spec_)
	{
		invalidate();
	};

	virtual ~OpenGLFramebuffer() override;

	virtual void bind() override;
	virtual void unbind() override;

	virtual void resize(uint32_t width, uint32_t height) override;

	virtual uint32_t get_color_attachment_id() override { return color_attachment; }

	virtual const FramebufferSpecification& get_spec() const override { return spec; };

private:
	void invalidate();

private:
	FramebufferSpecification spec;
	uint32_t renderer_id;
	uint32_t color_attachment;
	uint32_t depth_attachment;
};

} // namespace Neurex
