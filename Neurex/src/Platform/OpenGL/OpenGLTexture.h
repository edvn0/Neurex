#pragma once

#include "Neurex/renderer/Texture.h"
#include <glad/glad.h>

namespace Neurex {
class OpenGLTexture2D : public Texture2D {
public:
	explicit OpenGLTexture2D(const std::string& path);
	OpenGLTexture2D(uint32_t w, uint32_t h);
	virtual ~OpenGLTexture2D();

	virtual uint32_t get_width() const override { return width; };
	virtual uint32_t get_height() const override { return height; };

	virtual void set_data(void* data, uint32_t size) override;

	virtual void bind(uint32_t slot) const override;
	virtual void unbind() const override;

private:
	uint32_t width;
	uint32_t height;
	std::string path;
	uint32_t renderer_id;
	GLenum internal_format;
	GLenum data_format;
};

} // namespace Neurex
