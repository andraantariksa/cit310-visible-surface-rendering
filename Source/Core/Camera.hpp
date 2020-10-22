#ifndef __CORE_CAMERA_HPP
#define __CORE_CAMERA_HPP

#include <glm/glm.hpp>

class Camera
{
public:
	glm::vec3 m_Direction;

	Camera();
	~Camera() = default;
};

#endif
