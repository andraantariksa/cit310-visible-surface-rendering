#include "TransformSystem.hpp"

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "../Component/TransformComponent.hpp"
#include "../Component/SphereComponent.hpp"
#include "../Util/Logger.hpp"

#include <glm\gtx\string_cast.hpp>

TransformSystem::TransformSystem() :
	m_RotationAxis(0.0f),
	m_RotationDegree(0.0f),
	m_Translation(0.0f)
{
}

void TransformSystem::Translate(glm::vec3& translation)
{
		m_Translation = translation;
}

void TransformSystem::Rotate(const glm::vec3& rotationAxis, const float degree)
{
	m_RotationAxis = rotationAxis;
	m_RotationDegree = degree;
}

void TransformSystem::Update(entt::registry& registry)
{
	if (m_Translation != glm::vec3(0.0f))
	{
		registry.view<SphereComponent, TransformComponent>()
			.each([&](auto entity, SphereComponent& sphere, TransformComponent& transform)
				{
					transform.Translate(m_Translation);
				});

		m_Translation = glm::vec3(0.0f);
	}

	if (m_RotationDegree != 0.0f)
	{
		if (m_RotationAxis.x > 0.0f)
		{
			registry.view<SphereComponent, TransformComponent>()
				.each([&](auto entity, SphereComponent& sphere, TransformComponent& transform)
					{
						transform.RotateX(m_RotationDegree);
					});
		}
		else if (m_RotationAxis.y > 0.0f)
		{
			registry.view<SphereComponent, TransformComponent>()
				.each([&](auto entity, SphereComponent& sphere, TransformComponent& transform)
					{
						transform.RotateY(m_RotationDegree);
					});
		}
		else if (m_RotationAxis.z > 0.0f)
		{
			registry.view<SphereComponent, TransformComponent>()
				.each([&](auto entity, SphereComponent& sphere, TransformComponent& transform)
					{
						transform.RotateZ(m_RotationDegree);
					});
		}

		m_RotationDegree = 0.0f;
		m_RotationAxis = glm::vec3(0.0f);
	}
}
