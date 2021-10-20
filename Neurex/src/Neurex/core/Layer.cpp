#include "Layer.h"
#include "nxpch.h"

namespace Neurex {

Layer::Layer(const std::string& name)
	: debug_name(name){};

Layer::~Layer() = default;

}