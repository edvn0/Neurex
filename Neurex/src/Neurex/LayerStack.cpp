#include "nxpch.h"
#include "LayerStack.h"

namespace Neurex {

	LayerStack::LayerStack()
	{
		layer_insert = 0;
	}

	LayerStack::~LayerStack()
	{
		for (auto* l : layer_stack) {
			delete l;
		}
	}

	void LayerStack::push(Layer* layer)
	{
		layer_stack.emplace(begin() + layer_insert, layer);
		layer_insert++;
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
			layer_insert--;
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
