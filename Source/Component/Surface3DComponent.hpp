#ifndef __COMPONENT_SURFACE3DCOMPONENT_HPP
#define __COMPONENT_SURFACE3DCOMPONENT_HPP

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <array>
#include <utility>

class Surface3DComponent
{
public:
	// Because the surface is a triangle
	std::array<std::size_t, 3> m_VertexIndices;
	sf::Color m_Color;

	Surface3DComponent() = default;
	Surface3DComponent(std::array<std::size_t, 3>& vertexIndices, const sf::Color& color = sf::Color::Red);
	~Surface3DComponent() = default;
};

#endif
