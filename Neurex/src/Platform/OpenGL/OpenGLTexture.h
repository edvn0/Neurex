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

	virtual uint32_t get_renderer_id() const override { return renderer_id; };

	virtual bool compare(const Texture& other) const override
	{
		auto other_rid = static_cast<const OpenGLTexture2D&>(other).renderer_id;
		return renderer_id == other_rid;
	};

	virtual bool operator==(const Texture& other) const override
	{
		auto other_rid = static_cast<const OpenGLTexture2D&>(other).renderer_id;
		return renderer_id == other_rid;
	}

	virtual const std::string& get_name() const override { return name; }

private:
	uint32_t width;
	uint32_t height;
	std::string path;
	std::string name;
	uint32_t renderer_id;
	GLenum internal_format;
	GLenum data_format;
};

class OpenGLSpritesheetTexture : public SpritesheetTexture {
public:
	explicit OpenGLSpritesheetTexture(const ref<Texture2D>& base_texture, const glm::vec2& coords,
		const glm::vec2& cell_size, glm::vec2 sprite_size);

	~OpenGLSpritesheetTexture();

	const ref<Texture2D> get_base_texture() const override { return base; };

	const glm::vec2* get_texture_coordinates() const override { return tex_coords; };

private:
	ref<Texture2D> base;

	glm::vec2 tex_coords[4];
};

} // namespace Neurex
