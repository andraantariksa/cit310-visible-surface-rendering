#include "Surface3DComponent.hpp"

#include <glm/glm.hpp>

Surface3DComponent::Surface3DComponent(std::array<std::size_t, 3>& vertexIndices, const sf::Color& color):
	m_VertexIndices(vertexIndices),
	m_Color(color)
{
}
