#ifndef __COMPONENT_SURFACE2DCOMPONENT_HPP
#define __COMPONENT_SURFACE2DCOMPONENT_HPP

#include <SFML/Graphics.hpp>
#include <array>

class Surface2DComponent
{
public:
	std::array<sf::Vector2f, 3> m_Vertices;
	sf::Color m_Color;

	Surface2DComponent() = default;
	Surface2DComponent(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c, const sf::Color& color = sf::Color::Green);
	~Surface2DComponent() = default;
};

#endif
