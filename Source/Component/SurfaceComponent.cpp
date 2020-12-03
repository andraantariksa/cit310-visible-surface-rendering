#include "SurfaceComponent.hpp"

#include <glm/glm.hpp>

SurfaceComponent::SurfaceComponent(const glm::dvec3& a, const glm::dvec3& b, const glm::dvec3& c, const sf::Color& color):
	m_Vertices{a, b, c },
	m_Color(color)
{
}