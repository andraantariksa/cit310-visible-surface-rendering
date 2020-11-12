#ifndef __COMPONENT_TRANSFORMCOMPONENT_HPP
#define __COMPONENT_TRANSFORMCOMPONENT_HPP

#include <glm/glm.hpp>

class TransformComponent final
{
public:
	glm::mat4 m_MatTransform; // OCS to WCS transformation matrix
	glm::mat4 m_MatTranslation;
	glm::mat4 m_MatRotation;

	TransformComponent() noexcept;
	~TransformComponent() = default;
	TransformComponent& operator=(const TransformComponent&) noexcept;
};

#endif
