#include "SphereComponent.hpp"

#include <vector>
#include <iostream>
#include <glm/glm.hpp>

#define PI 3.14159265358979323846
#define RAD 0.0174533f

float map(float val, float start1, float end1, float start2, float end2)
{
	return ((val - start1) / (end1 - start1)) * (end2 - start2) + start2;
}

SphereComponent::SphereComponent(float r):
	m_R(r),
	m_MatTransform() // Identity matrix
{
	std::cout << "->" << m_Latitude << "-" << m_Longitude << "\n";
	m_Coord = std::vector(m_Latitude + 1, std::vector<glm::vec3>(m_Longitude + 1));
	for (std::size_t latitude_iter = 0; latitude_iter < m_Latitude + 1; latitude_iter++)
	{
		const std::size_t latitude = map((float)latitude_iter, 0.0f, (float)m_Latitude, 0.0f, PI);
		for (std::size_t longitude_iter = 0; longitude_iter < m_Longitude + 1; longitude_iter++)
		{
			const std::size_t longitude = map((float)longitude_iter, 0.0f, (float)m_Longitude, 0.0f, PI * 2);
			const float x = m_R * std::sin(latitude) * std::cos(longitude);
			const float y = m_R * std::sin(latitude) * std::sin(longitude);
			const float z = m_R * std::cos(latitude);
			m_Coord[latitude_iter][longitude_iter] = glm::vec3(x, y, z);
		}
	}
}

SphereComponent& SphereComponent::operator=(const SphereComponent&) noexcept
{
	return *this;
}
