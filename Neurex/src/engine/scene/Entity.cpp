#include "Entity.h"
#include "nxpch.h"

namespace Engine {

Entity::Entity(entt::entity handle, Scene* scene)
	: entity_handle(handle)
	, scene(scene)
{
}

}