#include "Surface2DComponent.hpp"

#include <glm/glm.hpp>

Surface2DComponent::Surface2DComponent(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const sf::Color& color) :
	m_Vertices{ a, b, c },
	m_Color(color)
{
}
