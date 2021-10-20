#pragma once

#include "Layer.h"
#include "Neurex/core/Core.h"

namespace Neurex {

class LayerStack {
	typedef std::vector<scoped<Layer>> stack;
	typedef std::vector<scoped<Layer>>::iterator stack_it;
	typedef std::vector<scoped<Layer>>::reverse_iterator stack_it_rev;

public:
	LayerStack();
	~LayerStack();

	void push(scoped<Layer> layer);
	void push_overlay(scoped<Layer> overlay);
	void pop(scoped<Layer> layer);
	void pop_overlay(scoped<Layer> overlay);

	stack_it begin() { return layer_stack.begin(); }
	stack_it end() { return layer_stack.end(); }
	stack_it_rev rbegin() { return layer_stack.rbegin(); };
	stack_it_rev rend() { return layer_stack.rend(); };

private:
	stack layer_stack;
	unsigned int layer_insert;
};

}
