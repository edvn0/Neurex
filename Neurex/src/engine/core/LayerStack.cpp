#include "LayerStack.h"
#include "nxpch.h"

namespace Engine {

LayerStack::LayerStack() { layer_insert = 0; }

LayerStack::~LayerStack() = default;

void LayerStack::push(scoped<Layer> layer)
{
	layer->attached();
	layer_stack.emplace(begin() + layer_insert, std::move(layer));
	layer_insert++;
}

void LayerStack::push_overlay(scoped<Layer> overlay)
{
	overlay->attached();
	layer_stack.emplace_back(std::move(overlay));
}

void LayerStack::pop(scoped<Layer> layer)
{
	auto found = std::find(begin(), end(), layer);
	if (found != end()) {
		layer_stack.erase(found);
		layer_insert--;
	}
}

void LayerStack::pop_overlay(scoped<Layer> overlay)
{
	auto found = std::find(begin(), end(), overlay);
	if (found != end()) {
		layer_stack.erase(found);
	}
}

}
