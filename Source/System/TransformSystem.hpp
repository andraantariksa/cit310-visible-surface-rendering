#ifndef __CORE_SYSTEM_TRANSFORMSYSTEM_HPP
#define __CORE_SYSTEM_TRANSFORMSYSTEM_HPP

#include <entt/entt.hpp>
#include <glm/glm.hpp>

// It's only for the sphere. Should we name it to TransformSphereSystem?
class TransformSystem
{
public:
	TransformSystem() = default;
	~TransformSystem() = default;
	void Update(entt::registry& registry, glm::vec3* translation);
};

#endif
