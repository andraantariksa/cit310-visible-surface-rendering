#ifndef __SYSTEM_TRANSFORMSYSTEM_HPP
#define __SYSTEM_TRANSFORMSYSTEM_HPP

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "../Component/TransformComponent.hpp"

class TransformSystem
{
public:
	TransformSystem() = default;
	~TransformSystem() = default;

	void Reset(entt::registry& registry, entt::entity& entity);

	void Rotate(entt::registry& registry, entt::entity& entity, glm::dvec3 const& rotateVec, double rotationDegree);

	void Translate(entt::registry& registry, entt::entity& entity, glm::dvec3 const& translateVec);

	void UpdateMatTransform(entt::registry& registry, entt::entity& entity);
};

#endif
