#include "SphereComponent.hpp"

#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <cmath>

#include "../Util/Logger.hpp"

#define PIDiv2 1.5707963267948966192313216916397f
#define PI 3.14159265358979323846f
#define PIMul2 6.2831853071795864769252867665f

SphereComponent::SphereComponent(float r, size_t nLongitude, size_t nLatitude) :
	m_R(r),
	m_NLongitude(nLongitude),
	m_NLatitude(nLatitude),
	m_IsCullBackface(true)
{
	RegenerateVertices();
}

SphereComponent& SphereComponent::operator=(const SphereComponent&) noexcept
{
	return *this;
}

// Call this function after you modify the radius, longitude, or latitude
void SphereComponent::RegenerateVertices()
{
	m_Vertices.resize(m_NLongitude * (m_NLatitude * 2 - 1) + 2);

	const float longitudeRad = PIMul2 / m_NLongitude;
	const float latitudeRad = PIDiv2 / m_NLatitude;

	size_t idx = 0;
	m_Vertices[idx++] = glm::vec4(0.0f, m_R, 0.0f, 1.0f);
	for (int latitude = (int)m_NLatitude - 1; latitude >= -((int)m_NLatitude - 1); --latitude)
	{
		for (int longitude = 0; longitude < m_NLongitude; ++longitude)
		{
			const float latitudeAngle = latitudeRad * latitude;
			const float longitudeAngle = longitudeRad * longitude;
			const float cosLatitude = cosf(latitudeAngle);
			const float x = cosLatitude * sinf(longitudeAngle);
			const float y = sinf(latitudeAngle);
			const flaot z = cosLatitude * cosf(longitudeAngle);

			m_Vertices[idx++] =
				glm::vec4(
					m_R,
					m_R,
					m_R,
					1.0f
				)
				*
				glm::vec4(
					x,
					y,
					z,
					1.0f
				);
		}
	}
	m_Vertices[idx++] = glm::vec4(0.0f, -m_R, 0.0f, 1.0f);
}
