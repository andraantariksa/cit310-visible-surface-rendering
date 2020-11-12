#include "TransformSystem.hpp"

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#define PI 3.14159265358979323846f

static inline float Deg2Rad(float deg)
{
	return deg * PI / 180.0f;
}

void TransformSystem::Reset(entt::registry& registry, entt::entity& entity)
{
	TransformComponent& transformComponent = registry.get<TransformComponent>(entity);
	transformComponent.m_MatTransform = glm::mat4(1.0f);
	transformComponent.m_MatRotation = glm::mat4(1.0f);
	transformComponent.m_MatTranslation = glm::mat4(1.0f);
}

void TransformSystem::Rotate(entt::registry& registry, entt::entity& entity, glm::vec3& rotateVec, float rotationDegree)
{
	TransformComponent& transformComponent = registry.get<TransformComponent>(entity);
	transformComponent.m_MatRotation *= glm::rotate(glm::mat4(1.0f), Deg2Rad(rotationDegree), rotateVec);
	UpdateMatTransform(registry, entity);
}

void TransformSystem::Translate(entt::registry& registry, entt::entity& entity, glm::vec3& translateVec)
{
	TransformComponent& transformComponent = registry.get<TransformComponent>(entity);
	transformComponent.m_MatTranslation *= glm::translate(glm::mat4(1.0f), translateVec);
	UpdateMatTransform(registry, entity);
}

void TransformSystem::UpdateMatTransform(entt::registry& registry, entt::entity& entity)
{
	TransformComponent& transformComponent = registry.get<TransformComponent>(entity);
	transformComponent.m_MatTransform = transformComponent.m_MatTranslation * transformComponent.m_MatRotation;
}
