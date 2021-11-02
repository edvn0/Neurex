#include "Entity.h"
#include "nxpch.h"

namespace Neurex {

Entity::Entity(entt::entity handle, Scene* scene)
	: entity_handle(handle)
	, scene(scene)
{
}

}