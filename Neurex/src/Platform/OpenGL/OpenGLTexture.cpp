#include "OpenGLTexture.h"
#include "nxpch.h"

#include "stb_image.h"
#include <glad/glad.h>

namespace Neurex {

OpenGLTexture2D::OpenGLTexture2D(const std::string& path_)
	: path(path_)
{
	stbi_set_flip_vertically_on_load(1);

	int w, h, channels;
	auto* data = stbi_load(path.c_str(), &w, &h, &channels, 0);

	NX_CORE_ASSERT(data, "Failed to load image!");
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
	glBindTexture(GL_TEXTURE_2D, renderer_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, data_format, GL_UNSIGNED_BYTE, data);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
};

OpenGLTexture2D::~OpenGLTexture2D()
{
	glDeleteTextures(1, &renderer_id);
}

void OpenGLTexture2D::bind(uint32_t slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, renderer_id);
};

} // namespace Neurex