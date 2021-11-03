#include "Layer.h"
#include "nxpch.h"

namespace Engine {

Layer::Layer(const std::string& name)
	: debug_name(name){};

Layer::~Layer() = default;

}