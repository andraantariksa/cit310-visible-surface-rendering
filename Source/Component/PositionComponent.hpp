#ifndef __COMPONENT_POSITIONCOMPONENT_HPP
#define __COMPONENT_POSITIONCOMPONENT_HPP

#include <glm/glm.hpp>

class PositionComponent final
{
public:
	glm::vec3 m_Position;

	PositionComponent(glm::vec3 position) noexcept;
	~PositionComponent() = default;
	PositionComponent& operator=(const PositionComponent&) noexcept;
};

#endif
