#include "RenderSystem.hpp"

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <cmath>

#include "../Component/PositionComponent.hpp"
#include "../Component/SphereComponent.hpp"

#define PI 3.14159265358979323846
#define RAD 0.0174533f

void RenderSystem::Render(entt::registry& registry, sf::RenderWindow& window)
{
	registry.view<SphereComponent, PositionComponent>().each([&](auto entity, SphereComponent sphere, PositionComponent pos)
	{
			//sf::ConvexShape triangleBuffer(3);
			//triangleBuffer.setPoint(0, sf::Vector2f(globeCoord[0][0].x, globeCoord[0][0].y));
			//triangleBuffer.setPoint(0, sf::Vector2f(globeCoord[1][0].x, globeCoord[1][0].y));
			//triangleBuffer.setPoint(0, sf::Vector2f(globeCoord[2][0].x, globeCoord[2][0].y));
			//window.draw(triangleBuffer);

			auto circle = sf::CircleShape(2.0f);
			circle.setFillColor(sf::Color::Red);
			for (std::size_t latitude_iter = 0; latitude_iter < sphere.m_Latitude; latitude_iter++)
			{
				for (std::size_t longitude_iter = 0; longitude_iter < sphere.m_Longitude; longitude_iter++)
				{
					//std::cout << "X " << sphere.m_Coord[latitude_iter][longitude_iter].x + 300.0f << " Y " << sphere.m_Coord[latitude_iter][longitude_iter].y + 300.0f << " \n";
					circle.setPosition(sf::Vector2(sphere.m_Coord[latitude_iter][longitude_iter].x + 300.0f, sphere.m_Coord[latitude_iter][longitude_iter].y + 300.0f));
					window.draw(circle);
					//triangleBuffer.setPoint(0, sf::Vector2f(globeCoord[latitude_iter + 1][longitude_iter].x, globeCoord[latitude_iter + 1][longitude_iter].y));
					//triangleBuffer.setPoint(0, sf::Vector2f(globeCoord[latitude_iter + 2][longitude_iter].x, globeCoord[latitude_iter + 2][longitude_iter].y));
					//triangleBuffer.setPoint(0, sf::Vector2f(globeCoord[latitude_iter + 3][longitude_iter].x, globeCoord[latitude_iter + 3][longitude_iter].y));
					//window.draw(triangleBuffer);
				}
			}
	});
}
