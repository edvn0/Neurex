#pragma once

namespace Engine {

enum class FramebufferTextureFormat {
	None = 0,

	// Color
	RGBA8,
	RED_INTEGER,

	// Depth/stencil
	DEPTH24STENCIL8,

	// Defaults
	Depth = DEPTH24STENCIL8
};

struct FramebufferTextureSpecification {
	FramebufferTextureSpecification() = default;
	FramebufferTextureSpecification(FramebufferTextureFormat format)
		: texture_format(format)
	{
	}

	FramebufferTextureFormat texture_format = FramebufferTextureFormat::None;
	// TODO: filtering/wrap
};

struct FramebufferAttachmentSpecification {
	FramebufferAttachmentSpecification() = default;
	FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments_)
		: attachments(attachments_)
	{
	}

	std::vector<FramebufferTextureSpecification> attachments;
};

struct FramebufferSpecification {
	uint32_t width, height;
	uint32_t samples;
	FramebufferAttachmentSpecification attachments;
	bool swap_chain_target = false;
};

class Framebuffer {
public:
	static ref<Framebuffer> create(const FramebufferSpecification& spec);

	virtual ~Framebuffer(){};

	virtual void bind() = 0;
	virtual void unbind() = 0;

	virtual void resize(uint32_t width, uint32_t height) = 0;

	virtual int read_pixel(uint32_t attachmentIndex, int x, int y) = 0;

	virtual void clear_attachment(uint32_t attachmentIndex, int value) = 0;

	virtual uint32_t get_color_attachment_id(uint32_t id = 0) = 0;

	virtual const FramebufferSpecification& get_spec() const = 0;
};

} // namespace Engine
