#include "TransformComponent.hpp"

#include "../Util/Logger.hpp"

TransformComponent::TransformComponent() noexcept:
	m_MatRotation(1.0f),
	m_MatTranslation(1.0f),
	m_MatTransform(1.0f)
{
}

TransformComponent& TransformComponent::operator=(const TransformComponent&) noexcept
{
	return *this;
}
