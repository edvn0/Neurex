#pragma once

#include <utility>

namespace Neurex {

class GPUDevice {
public:
	static std::pair<int, int> get_framebuffer_max_size();
};

} // namespace Neurex
