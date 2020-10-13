#include "PositionComponent.hpp"

PositionComponent::PositionComponent(float x, float y, float z) noexcept
	: x(x),
	  y(y),
	  z(z)
{
}

PositionComponent& PositionComponent::operator=(const PositionComponent&) noexcept
{
	return *this;
}
