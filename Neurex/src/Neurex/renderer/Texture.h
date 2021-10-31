#pragma once

#include "Neurex/core/Core.h"
#include <glm/glm.hpp>

namespace Neurex {

class Texture {
public:
	virtual ~Texture() = default;
	virtual uint32_t get_width() const = 0;
	virtual uint32_t get_height() const = 0;

	virtual void set_data(void* data, uint32_t size) = 0;

	virtual void bind(uint32_t slot = 0) const = 0;
	virtual void unbind() const = 0;

	virtual uint32_t get_renderer_id() const = 0;

	virtual bool compare(const Texture& other) const = 0;
	virtual bool operator==(const Texture& other) const = 0;
};

class Texture2D : public Texture {
public:
	static ref<Texture2D> create(const std::string& path);
	static ref<Texture2D> create(uint32_t width, uint32_t height);

	virtual const std::string& get_name() const = 0;
};

class SpritesheetTexture {
public:
	static ref<SpritesheetTexture> create(const ref<Texture2D>& base_texture, const glm::vec2& coords,
		const glm::vec2& cell_size, glm::vec2 sprite_size = { 1, 1 });

	virtual const ref<Texture2D> get_base_texture() const = 0;
	virtual const glm::vec2* get_texture_coordinates() const = 0;
};

} // namespace Neurex
