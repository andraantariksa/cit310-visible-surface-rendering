#include "RenderSystem.hpp"

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <cmath>

#include "../Component/PositionComponent.hpp"
#include "../Component/SphereComponent.hpp"
#include "../Util/Logger.hpp"
#include <glm/gtx/string_cast.hpp>

#define PI 3.14159265358979323846
#define RAD 0.0174533f

RenderSystem::RenderSystem():
	m_MatTransform(1.0f)
{
	const float scaleX = 1.0f;
	const float scaleY = 1.0f;
	const float translateX = 300.0f;
	const float translateY = 300.0f;

	glm::mat4x4 matScreenTransform(
		glm::vec4(scaleX, 0.0f, 0.0f, translateX),
		glm::vec4(0.0f, scaleY, 0.0f, translateY),
		glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);
	m_MatTransform *= matScreenTransform;
}

void RenderSystem::Render(entt::registry& registry, sf::RenderWindow& window)
{
	registry.view<SphereComponent, PositionComponent>().each([&](auto entity, SphereComponent sphere, PositionComponent pos)
	{
			auto circle = sf::CircleShape(2.0f);
			circle.setFillColor(sf::Color::Red);
			for (std::size_t latitude_iter = 0; latitude_iter < sphere.m_Latitude; latitude_iter++)
			{
				for (std::size_t longitude_iter = 0; longitude_iter < sphere.m_Longitude; longitude_iter++)
				{
 					circle.setPosition(Normalize3DToProjection(sphere.m_Coord[latitude_iter][longitude_iter]));
					window.draw(circle);
				}
			}
	});
}

inline sf::Vector2f RenderSystem::Normalize3DToProjection(const glm::vec4& v)
{
	const glm::vec4 result = v * m_MatTransform;
	return sf::Vector2f(result.x, result.y);
}
