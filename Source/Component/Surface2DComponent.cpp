#include "Surface2DComponent.hpp"

#include <glm/glm.hpp>

Surface2DComponent::Surface2DComponent(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c, const sf::Color& color) :
	m_Vertices{ a, b, c },
	m_Color(color)
{
}
