#include "TransformSystem.hpp"

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "../Component/PositionComponent.hpp"
#include "../Component/SphereComponent.hpp"

void TransformSystem::Update(entt::registry& registry, glm::vec3* translation)
{
	if (translation)
	{
		registry.view<SphereComponent, PositionComponent>()
			.each([&](auto entity, SphereComponent sphere, PositionComponent position)
			{
				position.m_Position += *translation;
			});
	}
}
