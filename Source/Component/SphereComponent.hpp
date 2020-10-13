#ifndef __COMPONENT_SPHERECOMPONENT_HPP
#define __COMPONENT_SPHERECOMPONENT_HPP

#include <glm/glm.hpp>
#include <cstdint>
#include <vector>

class SphereComponent
{
public:
	float m_R;
	std::uint32_t m_Latitude = 30;
	std::uint32_t m_Longitude = 30;

	glm::mat4x4 m_MatTransform;

	std::vector<std::vector<glm::vec3>> m_Coord;

	SphereComponent(float r);
	~SphereComponent() = default;
	SphereComponent& operator=(const SphereComponent&) noexcept;
};

#endif
