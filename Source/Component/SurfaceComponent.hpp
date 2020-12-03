#ifndef __COMPONENT_SURFACECOMPONENT_HPP
#define __COMPONENT_SURFACECOMPONENT_HPP

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <array>

class SurfaceComponent
{
public:
	// Because the surface is a triangle
	std::array<glm::dvec3, 3> m_Vertices;
	sf::Color m_Color;

	SurfaceComponent() = default;
	SurfaceComponent(const glm::dvec3& a, const glm::dvec3& b, const glm::dvec3& c, const sf::Color& color = sf::Color::Green);
	~SurfaceComponent() = default;
};

#endif
