#include "SphereComponent.hpp"

#include <vector>
#include <iostream>
#include <glm/glm.hpp>

#include "../Util/Logger.hpp"
#include <glm/gtx/string_cast.hpp>

#define PI 3.14159265358979323846
#define RAD 0.0174533f

inline float map(float val, float start1, float end1, float start2, float end2)
{
	return ((val - start1) / (end1 - start1)) * (end2 - start2) + start2;
}

SphereComponent::SphereComponent(float r):
	m_R(r),
	m_MatTransform(1.0f) // Identity matrix
{
	//std::cout << glm::to_string(m_MatTransform) << '\n';
	//std::cout << glm::to_string(m_MatTransform) << '\n';

	m_Coord = std::vector(m_Latitude + 1ul, std::vector<glm::vec4>(m_Longitude + 1ul));
	for (std::size_t latitude_iter = 0; latitude_iter <= m_Latitude; ++latitude_iter)
	{
		const float latitude = map((float)latitude_iter, 0.0f, (float)m_Latitude, 0.0f, PI);
		for (std::size_t longitude_iter = 0; longitude_iter <= m_Longitude; ++longitude_iter)
		{
			const float longitude = map((float)longitude_iter, 0.0f, (float)m_Longitude, 0.0f, PI * 2);
			const float x = m_R * std::sinf(latitude) * std::cosf(longitude);
			const float y = m_R * std::sinf(latitude) * std::sinf(longitude);
			const float z = m_R * std::cosf(latitude);
			m_Coord[latitude_iter][longitude_iter] = glm::vec4(x, y, z, 1.0f);
		}
	}
}

SphereComponent& SphereComponent::operator=(const SphereComponent&) noexcept
{
	return *this;
}

void SphereComponent::Transform()
{

}

void SphereComponent::Translate(glm::vec3 translation)
{
	glm::mat4x4 translationMatrix(1.0f);
	translationMatrix[3] = glm::vec4(translation, 0.0f);
	m_MatTransform *= translationMatrix;
}

void SphereComponent::RotateX(float rad)
{
	glm::mat4x4 rotationMatrix(
		glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, std::cosf(rad), std::sinf(rad), 0.0f),
		glm::vec4(0.0f, -std::sinf(rad), std::cos(rad), 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);
	m_MatTransform *= rotationMatrix;
}

void SphereComponent::RotateY(float rad)
{
	glm::mat4x4 rotationMatrix(
		glm::vec4(std::cosf(rad), 0.0f, std::sinf(rad), 0.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
		glm::vec4(-std::sinf(rad), 0.0f, std::cosf(rad), 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);
	m_MatTransform *= rotationMatrix;
}

void SphereComponent::RotateZ(float rad)
{
	glm::mat4x4 rotationMatrix(
		glm::vec4(std::cosf(rad), -std::sinf(rad), 0.0f, 0.0f),
		glm::vec4(std::sinf(rad), -std::cosf(rad), 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);
	m_MatTransform *= rotationMatrix;
}
