#include "OpenGLTexture.h"
#include "nxpch.h"

#include "stb_image.h"
#include <glad/glad.h>

namespace Neurex {

OpenGLTexture2D::OpenGLTexture2D(uint32_t w, uint32_t h)
	: name("Unnamed empty texture")
{
	NX_PROFILE_FUNCTION();

	width = w;
	height = h;

	internal_format = GL_RGBA8;
	data_format = GL_RGBA;
	glGenTextures(1, &renderer_id);
	NX_CORE_INFO("Empty texture created, render_id: {0}", renderer_id);
	glBindTexture(GL_TEXTURE_2D, renderer_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

OpenGLTexture2D::OpenGLTexture2D(const std::string& path_)
	: path(path_)
{
	NX_PROFILE_FUNCTION();

	stbi_set_flip_vertically_on_load(1);

	std::filesystem::path path = path_;
	name = path.stem().string();

	int w, h, channels;
	stbi_uc* data = nullptr;
	{
		NX_PROFILE_SCOPE("OpenGLTexture2D::OpenGLTexture2D(const std::string& path)");
		data = stbi_load(path.c_str(), &w, &h, &channels, 0);
	}

	if (!data) {
		NX_CORE_ASSERT(false, "Failed to load image!");
	} else {
		width = w;
		height = h;

		GLenum internal_format_ = 0, data_format_ = 0;
		if (channels == 4) {
			internal_format_ = GL_RGBA8;
			data_format_ = GL_RGBA;
		} else if (channels == 3) {
			internal_format_ = GL_RGB8;
			data_format_ = GL_RGB;
		}

		internal_format = internal_format_;
		data_format = data_format_;

		glGenTextures(1, &renderer_id);
		NX_CORE_INFO("Texture created, render_id: {0}", renderer_id);
		glBindTexture(GL_TEXTURE_2D, renderer_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, data_format, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}
};

OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &renderer_id); }

void OpenGLTexture2D::bind(uint32_t slot) const
{
	NX_PROFILE_FUNCTION();
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, renderer_id);
};

void OpenGLTexture2D::unbind() const
{
	NX_PROFILE_FUNCTION();
	glBindTexture(GL_TEXTURE_2D, 0);
};

void OpenGLTexture2D::set_data(void* data, uint32_t size)
{
	NX_PROFILE_FUNCTION();
	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

OpenGLSpritesheetTexture::OpenGLSpritesheetTexture(
	const ref<Texture2D>& base_texture, const glm::vec2& coords, const glm::vec2& cell_size, glm::vec2 sprite_size)
	: base(base_texture)
{
	auto w = base_texture->get_width();
	auto h = base_texture->get_height();
	glm::vec2 min = { ((coords.x * cell_size.x) / w), ((coords.y * cell_size.y) / h) };
	glm::vec2 max
		= { (((coords.x + sprite_size.x) * cell_size.x) / w), (((coords.y + sprite_size.y) * cell_size.y) / h) };

	tex_coords[0] = { min.x, min.y };
	tex_coords[1] = { max.x, min.y };
	tex_coords[2] = { max.x, max.y };
	tex_coords[3] = { min.x, max.y };
}

OpenGLSpritesheetTexture::~OpenGLSpritesheetTexture() = default;

} // namespace Neurex