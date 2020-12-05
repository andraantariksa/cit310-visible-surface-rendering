#ifndef __COMPONENT_TRIANGLE2DCOMPONENT_HPP
#define __COMPONENT_TRIANGLE2DCOMPONENT_HPP

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <array>

class Triangle2DComponent
{
public:
	std::array<glm::dvec2, 3> m_Vertices;
	sf::Color m_Color;

	Triangle2DComponent() = default;
	Triangle2DComponent(const glm::dvec2& a, const glm::dvec2& b, const glm::dvec2& c, const sf::Color& color = sf::Color::Green);
	~Triangle2DComponent() = default;
};

#endif
