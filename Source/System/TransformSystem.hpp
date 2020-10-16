#ifndef __CORE_SYSTEM_TRANSFORMSYSTEM_HPP
#define __CORE_SYSTEM_TRANSFORMSYSTEM_HPP

#include <entt/entt.hpp>
#include <glm/glm.hpp>

// It's only for the sphere. Should we name it to TransformSphereSystem?
class TransformSystem
{
private:
	glm::vec3 m_Translation;
	glm::vec3 m_RotationAxis;
	float m_RotationDegree;

public:
	TransformSystem();
	~TransformSystem() = default;

	void Translate(glm::vec3& translation);
	void Rotate(const glm::vec3& rotationAxis, const float degree);
	void Update(entt::registry& registry);
};

#endif
