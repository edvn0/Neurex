#pragma once

#include "Neurex/core/Core.h"
#include "Layer.h"

namespace Neurex {

	class LayerStack
	{
		typedef std::vector<Layer*> stack;
		typedef std::vector<Layer*>::iterator stack_it;
		typedef std::vector<Layer*>::reverse_iterator stack_it_rev;
	public:
		LayerStack();
		~LayerStack();

		void push(Layer* layer);
		void push_overlay(Layer* overlay);
		void pop(Layer* layer);
		void pop_overlay(Layer* overlay);

		stack::iterator begin() { return layer_stack.begin(); }
		stack::iterator end() { return layer_stack.end(); }
	private:
		stack layer_stack;
		unsigned int layer_insert;
	};

}

