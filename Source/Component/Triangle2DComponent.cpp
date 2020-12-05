#include "Triangle2DComponent.hpp"

#include <glm/glm.hpp>

Triangle2DComponent::Triangle2DComponent(const glm::dvec2& a, const glm::dvec2& b, const glm::dvec2& c, const sf::Color& color) :
	m_Vertices{ a, b, c },
	m_Color(color)
{
}
