#ifndef __COMPONENT_SHAPE3DCOMPONENT_HPP
#define __COMPONENT_SHAPE3DCOMPONENT_HPP

#include <vector>

#include "SurfaceComponent.hpp"

class Shape3DComponent
{
public:
	std::vector<SurfaceComponent> m_Surfaces;

	Shape3DComponent() = default;
	Shape3DComponent(const std::vector<SurfaceComponent>& surfaces);
	~Shape3DComponent() = default;
};

#endif
