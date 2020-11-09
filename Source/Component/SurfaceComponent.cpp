#include "SurfaceComponent.hpp"

#include <glm/glm.hpp>

SurfaceComponent::SurfaceComponent(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const sf::Color& color):
	m_Vertices{a, b, c},
	m_Color(color)
{
}
