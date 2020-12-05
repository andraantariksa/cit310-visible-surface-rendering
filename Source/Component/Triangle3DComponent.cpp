#include "Triangle3DComponent.hpp"

#include <glm/glm.hpp>

Triangle3DComponent::Triangle3DComponent(const glm::dvec3& a, const glm::dvec3& b, const glm::dvec3& c, const sf::Color& color) :
	m_Vertices{ a, b, c },
	m_Color(color)
{
}
