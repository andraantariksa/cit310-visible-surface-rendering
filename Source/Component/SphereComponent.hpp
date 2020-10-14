#ifndef __COMPONENT_SPHERECOMPONENT_HPP
#define __COMPONENT_SPHERECOMPONENT_HPP

#include <glm/glm.hpp>
#include <cstdint>
#include <vector>

class SphereComponent
{
public:
	float m_R;
	std::uint32_t m_Latitude = 25;
	std::uint32_t m_Longitude = 25;

	glm::mat4x4 m_MatTransform;

	std::vector<std::vector<glm::vec4>> m_Coord;

	SphereComponent(float r);
	~SphereComponent() = default;
	SphereComponent& operator=(const SphereComponent&) noexcept;
	// You have to call Transform() everytime you have made any change
	void Transform();
	void Translate(glm::vec3 translation);
	void RotateX(float rad);
	void RotateY(float rad);
	void RotateZ(float rad);
};

#endif
