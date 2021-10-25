#include "MetalTexture.h"
#include "nxpch.h"

#include "stb_image.h"
#include <glad/glad.h>

namespace Neurex {

MetalTexture2D::MetalTexture2D(uint32_t w, uint32_t h)
{
}

MetalTexture2D::MetalTexture2D(const std::string& path_)
	: path(path_)
{
	int twidth, theight, channels;
	auto* data = stbi_load(path.c_str(), &twidth, &theight, &channels, 0);
	NX_CORE_ASSERT(data, "Failed to load data.");
	width = twidth;
	height = theight;

	glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id);
	glTextureStorage2D(renderer_id, 1, GL_RGB8, width, height);

	glTextureParameteri(renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(renderer_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureSubImage2D(renderer_id, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
};

MetalTexture2D::~MetalTexture2D()
{
	glDeleteTextures(1, &renderer_id);
}

void MetalTexture2D::bind(uint32_t slot) const
{
	glBindTextureUnit(slot, renderer_id);
};

} // namespace Neurex