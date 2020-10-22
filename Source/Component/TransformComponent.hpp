#ifndef __COMPONENT_POSITIONCOMPONENT_HPP
#define __COMPONENT_POSITIONCOMPONENT_HPP

#include <glm/glm.hpp>

class TransformComponent final
{
public:
	glm::mat4 m_MatTransform; // World transformation matrix

	TransformComponent() noexcept;
	TransformComponent(const glm::vec3& startingPosition) noexcept;
	~TransformComponent() = default;
	TransformComponent& operator=(const TransformComponent&) noexcept;

	void ResetMatrix();
	void Translate(const glm::vec3& translation);
	void RotateX(float deg);
	void RotateY(float deg);
	void RotateZ(float deg);
	void Rotate(float deg, glm::vec3& rotationAxis);
};

#endif
