#include "Shape3DComponent.hpp"

Shape3DComponent::Shape3DComponent(const std::vector<glm::dvec3>& vertices, const std::vector<Surface3DComponent>& surfaces):
	m_Surfaces(surfaces),
	m_Vertices(vertices)
{
}
