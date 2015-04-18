#pragma once

#include "PhysXMeshCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		PhysXMeshCollider::PhysXMeshCollider(WorldInterface *world, void *&data)
			: ColliderInterface(world, data), MeshColliderInterface(world, data), PhysXCollider(world, data, nullptr)
		{
			// TO_DO --> /!\ PhysXCollider last constructor parameter
			return;
		}
	}
}