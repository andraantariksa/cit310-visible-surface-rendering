#ifndef _COMPONENT_SURFACECOMPONENT_HPP
#define _COMPONENT_SURFACECOMPONENT_HPP

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>

class SurfaceComponent
{
public:
	// Because the surface is a triangle
	glm::vec3 m_Vertices[3];
	sf::Color m_Color;

	SurfaceComponent() = default;
	SurfaceComponent(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const sf::Color& color = sf::Color::Green);
	~SurfaceComponent() = default;
};

#endif
