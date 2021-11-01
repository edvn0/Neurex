#pragma once

namespace Neurex {

struct FramebufferSpecification {
	uint32_t width, height;
	uint32_t samples;

	bool swap_chain_target = false;
};

class Framebuffer {
public:
	static ref<Framebuffer> create(const FramebufferSpecification& spec);

	virtual ~Framebuffer(){};

	virtual void bind() = 0;
	virtual void unbind() = 0;

	virtual void resize(uint32_t width, uint32_t height) = 0;

	virtual uint32_t get_color_attachment_id() = 0;

	virtual const FramebufferSpecification& get_spec() const = 0;
};

} // namespace Neurex
