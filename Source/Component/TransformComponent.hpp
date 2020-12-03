#ifndef __COMPONENT_TRANSFORMCOMPONENT_HPP
#define __COMPONENT_TRANSFORMCOMPONENT_HPP

#include <glm/glm.hpp>

class TransformComponent final
{
public:
	glm::dmat4 m_MatTransform; // OCS to WCS transformation matrix
	glm::dmat4 m_MatTranslation;
	glm::dmat4 m_MatRotation;

	TransformComponent() noexcept;
	~TransformComponent() = default;
	TransformComponent& operator=(const TransformComponent&) noexcept;
};

#endif
