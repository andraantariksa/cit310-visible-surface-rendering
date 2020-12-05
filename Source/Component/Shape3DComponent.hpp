#ifndef __COMPONENT_SHAPE3DCOMPONENT_HPP
#define __COMPONENT_SHAPE3DCOMPONENT_HPP

#include <vector>
#include <utility>
#include <array>

#include "Surface3DComponent.hpp"

class Shape3DComponent
{
public:
	std::vector<glm::dvec3> m_Vertices;
	std::vector<Surface3DComponent> m_Surfaces;

	Shape3DComponent() = default;
	Shape3DComponent(const std::vector<glm::dvec3>& vertices, const std::vector<Surface3DComponent>& surfaces);
	~Shape3DComponent() = default;
};

#endif
