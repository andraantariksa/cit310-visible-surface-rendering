#ifndef __COMPONENT_SPHERECOMPONENT_HPP
#define __COMPONENT_SPHERECOMPONENT_HPP

#include <glm/glm.hpp>
#include <cstdint>
#include <vector>

class SphereComponent
{
public:
	float m_R;
	size_t m_NLongitude;
	size_t m_NLatitude;

	std::vector<std::vector<glm::vec4>> m_Coord;

	SphereComponent(float r, size_t longitude, size_t latitude);
	~SphereComponent() = default;
	SphereComponent& operator=(const SphereComponent&) noexcept;
};

#endif
