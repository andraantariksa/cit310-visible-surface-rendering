#include "PositionComponent.hpp"

PositionComponent::PositionComponent(glm::vec3 position) noexcept
	: m_Position(position)
{
}

PositionComponent& PositionComponent::operator=(const PositionComponent&) noexcept
{
	return *this;
}
