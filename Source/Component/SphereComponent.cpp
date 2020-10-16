#include "SphereComponent.hpp"

#include <vector>
#include <iostream>
#include <glm/glm.hpp>

#include "../Util/Logger.hpp"
#include <glm/gtx/string_cast.hpp>

#define PIDiv2 1.5707963267948966192313216916397f
#define PI 3.14159265358979323846f
#define PIMul2 6.2831853071795864769252867665f
#define RAD 0.0174533f

inline float map(float val, float start1, float end1, float start2, float end2)
{
	return ((val - start1) / (end1 - start1)) * (end2 - start2) + start2;
}

SphereComponent::SphereComponent(float r, size_t nLongitude, size_t nLatitude) :
	m_R(r),
	m_NLongitude(nLongitude),
	m_NLatitude(nLatitude)
{
	m_Coord = std::vector(m_NLatitude + 1ul, std::vector<glm::vec4>(m_NLongitude + 1ul));
	for (std::size_t latitude_iter = 0; latitude_iter <= m_NLatitude; ++latitude_iter)
	{
		const float latitude = map((float)latitude_iter, 0.0f, (float)m_NLatitude, 0.0f, PI);
		for (std::size_t longitude_iter = 0; longitude_iter <= m_NLongitude; ++longitude_iter)
		{
			const float longitude = map((float)longitude_iter, 0.0f, (float)m_NLongitude, 0.0f, PI * 2);
			const float sinLatitude = std::sinf(latitude);
			m_Coord[latitude_iter][longitude_iter] = 
				glm::vec4(
					m_R,
					m_R,
					m_R,
					1.0f
				)
				*
				glm::vec4(
					sinLatitude * std::cosf(longitude),
					sinLatitude * std::sinf(longitude),
					std::cosf(latitude),
					1.0f
				);
		}

		// UNDONE
		// Rewriting the vertex compute code
		const float radLongitude = PIMul2 / m_NLongitude;
		const float radLatitude = PIDiv2 / m_NLatitude;
	}
}

SphereComponent& SphereComponent::operator=(const SphereComponent&) noexcept
{
	return *this;
}
