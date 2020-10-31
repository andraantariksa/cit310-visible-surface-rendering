#include "TransformComponent.hpp"

#include <glm/gtx/transform.hpp>

#include "../Util/Logger.hpp"

#define PI 3.14159265358979323846f

TransformComponent::TransformComponent() noexcept:
	m_MatTransform(1.0f),
	m_MatTranslation(1.0f),
	m_MatRotation(1.0f)
{
}

static inline float deg2rad(float deg)
{
	return deg * PI / 180.0f;
}

void TransformComponent::ResetMatrix() {
    m_MatTransform = glm::mat4(1.0f);

}

TransformComponent::TransformComponent(const glm::vec3& startingPosition) noexcept :
	m_MatTransform(1.0f)
{
	Translate(startingPosition);
}

TransformComponent& TransformComponent::operator=(const TransformComponent&) noexcept
{
	return *this;
}

void TransformComponent::Translate(const glm::vec3& translation)
{
	m_MatTranslation *= glm::translate(glm::mat4(1.0f), translation);
	UpdateMatTransform();
}

void TransformComponent::RotateX(float deg)
{
	m_MatRotation *= glm::rotate(deg2rad(deg), glm::vec3(1.0f, 0.0f, 0.0f));
	UpdateMatTransform();
}

void TransformComponent::RotateY(float deg)
{
	m_MatRotation *= glm::rotate(deg2rad(deg), glm::vec3(0.0f, 1.0f, 0.0f));
	UpdateMatTransform();
}

void TransformComponent::RotateZ(float deg)
{
	m_MatRotation *= glm::rotate(deg2rad(deg), glm::vec3(0.0f, 0.0f, 1.0f));
	UpdateMatTransform();
}

void TransformComponent::Rotate(float deg, glm::vec3& rotationAxis)
{
	m_MatRotation *= glm::rotate(deg2rad(deg), rotationAxis);
	UpdateMatTransform();
}

void TransformComponent::UpdateMatTransform()
{
	m_MatTransform = m_MatTranslation * m_MatRotation;
}
