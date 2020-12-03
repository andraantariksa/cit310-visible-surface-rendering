#include "TransformSystem.hpp"

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "../Util/Logger.hpp"

#define PI 3.14159265358979323846f

static inline double Deg2Rad(double deg)
{
	return deg * PI / 180.0;
}

void TransformSystem::Reset(entt::registry& registry, entt::entity& entity)
{
	TransformComponent& transformComponent = registry.get<TransformComponent>(entity);
	transformComponent.m_MatTransform = glm::dmat4(1.0);
	transformComponent.m_MatRotation = glm::dmat4(1.0);
	transformComponent.m_MatTranslation = glm::dmat4(1.0);
}

void TransformSystem::Rotate(entt::registry& registry, entt::entity& entity, glm::dvec3 const& rotateVec, double rotationDegree)
{
	TransformComponent& transformComponent = registry.get<TransformComponent>(entity);
	transformComponent.m_MatRotation *= glm::rotate(glm::dmat4(1.0), Deg2Rad(rotationDegree), rotateVec);
	UpdateMatTransform(registry, entity);
}

void TransformSystem::Translate(entt::registry& registry, entt::entity& entity, glm::dvec3 const& translateVec)
{
	TransformComponent& transformComponent = registry.get<TransformComponent>(entity);
	transformComponent.m_MatTranslation *= glm::translate(glm::dmat4(1.0), translateVec);
	UpdateMatTransform(registry, entity);
}

void TransformSystem::UpdateMatTransform(entt::registry& registry, entt::entity& entity)
{
	TransformComponent& transformComponent = registry.get<TransformComponent>(entity);
	transformComponent.m_MatTransform = transformComponent.m_MatTranslation * transformComponent.m_MatRotation;
}

#undef PI
