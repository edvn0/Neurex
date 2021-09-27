#pragma once

#include "Neurex/Core.h"
#include "Layer.h"

namespace Neurex {

	class NX_API LayerStack
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
		stack_it layer_stack_insert;
	};

}
