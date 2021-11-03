#pragma once

#include "Entity.h"

namespace Engine {

class ScriptableEntity {
public:
	virtual ~ScriptableEntity() { }

	template <typename T> T& get_component() { return entity.get_component<T>(); }

protected:
	virtual void on_create() { }
	virtual void on_destroy() { }
	virtual void on_update(Timestep ts) { }

private:
	Entity entity;
	friend class Scene;
};

}