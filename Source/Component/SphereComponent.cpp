#include "SphereComponent.hpp"

SphereComponent::SphereComponent(float r)
	: r(r)
{
}

SphereComponent& SphereComponent::operator=(const SphereComponent&) noexcept
{
	return *this;
}
