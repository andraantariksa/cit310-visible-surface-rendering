#ifndef __COMPONENT_SURFACE2DCOMPONENT_HPP
#define __COMPONENT_SURFACE2DCOMPONENT_HPP

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <array>

class Surface2DComponent
{
public:
	std::array<glm::vec2, 3> m_Vertices;
	sf::Color m_Color;

	Surface2DComponent() = default;
	Surface2DComponent(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const sf::Color& color = sf::Color::Green);
	~Surface2DComponent() = default;
};

#endif
