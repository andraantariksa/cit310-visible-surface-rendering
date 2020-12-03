#include "TransformComponent.hpp"

#include "../Util/Logger.hpp"

TransformComponent::TransformComponent() noexcept:
	m_MatRotation(1.0),
	m_MatTranslation(1.0),
	m_MatTransform(1.0)
{
}

TransformComponent& TransformComponent::operator=(const TransformComponent&) noexcept
{
	return *this;
}
