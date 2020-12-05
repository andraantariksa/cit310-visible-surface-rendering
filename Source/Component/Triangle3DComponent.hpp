#ifndef __COMPONENT_TRIANGLE3DCOMPONENT_HPP
#define __COMPONENT_TRIANGLE3DCOMPONENT_HPP

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <array>

class Triangle3DComponent
{
public:
	std::array<glm::dvec3, 3> m_Vertices;
	sf::Color m_Color;

	Triangle3DComponent() = default;
	Triangle3DComponent(const glm::dvec3& a, const glm::dvec3& b, const glm::dvec3& c, const sf::Color& color = sf::Color::Green);
	~Triangle3DComponent() = default;
};

#endif
