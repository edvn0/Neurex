#include "nxpch.h"
#include "LayerStack.h"

namespace Neurex {

	LayerStack::LayerStack()
	{
		layer_stack_insert = layer_stack.begin();
	}

	LayerStack::~LayerStack()
	{
		for (auto* l : layer_stack) {
			delete l;
		}
	}

	void LayerStack::push(Layer* layer)
	{
		layer_stack_insert = layer_stack.emplace(layer_stack_insert, layer);
	}

	void LayerStack::push_overlay(Layer* overlay)
	{
		layer_stack.emplace_back(overlay);
	}

	void LayerStack::pop(Layer* layer)
	{
		auto found = std::find(begin(), end(), layer);
		if (found != end()) {
			layer_stack.erase(found);
			layer_stack_insert--;
		}
	}

	void LayerStack::pop_overlay(Layer* overlay)
	{
		auto found = std::find(begin(), end(), overlay);
		if (found != end()) {
			layer_stack.erase(found);
		}
	}

}
