#include "RenderSystem.hpp"

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <cmath>
#include <optional>

#include "../Macro.hpp"
#include "../Component/TransformComponent.hpp"
#include "../Component/Shape3DComponent.hpp"
#include "../Core/Camera.hpp"
#include "../Util/Logger.hpp"

#define PI 3.14159265358979323846f
#define RAD 0.0174533f

RenderSystem::RenderSystem()
{
}

void RenderSystem::Render(entt::registry& registry, sf::RenderWindow& window)
{
	registry.view<Shape3DComponent, TransformComponent>().each([&](auto entity, Shape3DComponent& sphere, const TransformComponent& transform)
		{
			// Draw the vertices
			/*
			auto circle = sf::CircleShape(2.0f);
			circle.setFillColor(sf::Color::Red);
			for (glm::vec4& vertex : m_EntitySphere.m_Vertices)
			{
				auto s = Normalize3DToProjectionSFML(vertex, transform);
				circle.setPosition(s);
				window.draw(circle);
			}
			*/

			// TODO
			// Parallelize the code?
			// https://en.cppreference.com/w/cpp/algorithm/execution_policy_tag_t
			// https://docs.microsoft.com/en-us/cpp/parallel/concrt/how-to-write-a-parallel-for-loop?view=vs-2019

			for (SurfaceComponent& surface : sphere.m_Surfaces)
			{
				glm::vec3 v1 = surface.m_Vertices[1] - surface.m_Vertices[0];
				glm::vec3 v2 = surface.m_Vertices[2] - surface.m_Vertices[1];
				if (glm::dot(glm::cross(v1, v2), m_Camera.m_Direction) < 0.0f)
				{
					// TODO
					
				}
			}
		});
}
