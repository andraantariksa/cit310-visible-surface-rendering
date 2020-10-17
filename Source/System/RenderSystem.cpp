#include "RenderSystem.hpp"

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <cmath>

#include "../Component/TransformComponent.hpp"
#include "../Component/SphereComponent.hpp"
#include <glm/gtx/string_cast.hpp>

#define PI 3.14159265358979323846f
#define RAD 0.0174533f

RenderSystem::RenderSystem():
	m_MatTransform(1.0f)
{
	const float scaleX = 1.0f;
	const float scaleY = 1.0f;
	const float translateX = 300.0f;
	const float translateY = 300.0f;

	m_MatTransform *= glm::mat4(
		glm::vec4(scaleX, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, -scaleY, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
		glm::vec4(translateX, translateY, 0.0f, 1.0f)
	);
}

void RenderSystem::Render(entt::registry& registry, sf::RenderWindow& window)
{
	registry.view<SphereComponent, TransformComponent>().each([&](auto entity, SphereComponent& sphere, const TransformComponent& transform)
	{
			// Draw the vertex
			/*
			auto circle = sf::CircleShape(2.0f);
			circle.setFillColor(sf::Color::Red);
			for (glm::vec4& vertex : m_EntitySphere.m_Vertices)
			{
				auto s = Normalize3DToProjection(vertex, transform);
				circle.setPosition(s);
				window.draw(circle);
			}
			*/
			
			// TODO
			// Parallelize the code?
			// https://en.cppreference.com/w/cpp/algorithm/execution_policy_tag_t
			// https://docs.microsoft.com/en-us/cpp/parallel/concrt/how-to-write-a-parallel-for-loop?view=vs-2019

			// In case you want to use surface
			//sf::ConvexShape convex(3);
			//convex.setFillColor(sf::Color::Green);
			//convex.setOutlineColor(sf::Color::Green);
			//convex.setOutlineThickness(1.0f);

			// Toggle backface cull
			//if (sphere.m_IsBackfaceCull)
			//{
			
			//}
			//else
			//{
			
			//}

			// Bottom part
			for (size_t i = 0; i < sphere.m_NLongitude - 1; ++i)
			{
				const glm::vec3 v1 = sphere.m_Vertices[i + 1] - sphere.m_Vertices[0];
				const glm::vec3 v2 = sphere.m_Vertices[i + 2] - sphere.m_Vertices[i + 1];
				if (glm::cross(v1, v2).z > 0.0f)
				{
					sf::Vertex line[] =
					{
						sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[0], transform)),
						sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[i + 1], transform)),
						sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[i + 1], transform)),
						sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[i + 2], transform)),
						sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[i + 2], transform)),
						sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[0], transform))
					};
					window.draw(line, 6, sf::Lines);

					// In case you want to use surface
					//convex.setPoint(0, Normalize3DToProjection(m_EntitySphere.m_Vertices[0], transform));
					//convex.setPoint(1, Normalize3DToProjection(m_EntitySphere.m_Vertices[i + 1], transform));
					//convex.setPoint(2, Normalize3DToProjection(m_EntitySphere.m_Vertices[i + 2], transform));
					//window.draw(convex);
				}
			}


			const glm::vec3 v1 = sphere.m_Vertices[sphere.m_NLongitude] - sphere.m_Vertices[0];
			const glm::vec3 v2 = sphere.m_Vertices[1] - sphere.m_Vertices[sphere.m_NLongitude];
			if (glm::cross(v1, v2).z > 0.0f)
			{
				sf::Vertex line[] =
				{
					sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[0], transform)),
					sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[sphere.m_NLongitude], transform)),
					sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[sphere.m_NLongitude], transform)),
					sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[1], transform)),
					sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[1], transform)),
					sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[0], transform))
				};
				window.draw(line, 6, sf::Lines);
			}

			// Middle part
			for (size_t i = 1; i < sphere.m_NLatitude * 2 - 1; ++i)
			{
				for (size_t j = 0; j < sphere.m_NLongitude - 1; ++j)
				{
					const glm::vec3 v1 = sphere.m_Vertices[sphere.m_NLongitude * i + j + 2] - sphere.m_Vertices[sphere.m_NLongitude * i + j + 1];
					const glm::vec3 v2 = sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1] - sphere.m_Vertices[sphere.m_NLongitude * i + j + 2];
					if (glm::cross(v1, v2).z > 0.0f)
					{
						sf::Vertex line[] =
						{
							sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[sphere.m_NLongitude * i + j + 1], transform)),
							sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform)),
							sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform)),
							sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform)),
							sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform)),
							sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[sphere.m_NLongitude * i + j + 1], transform)),
						};
						window.draw(line, 6, sf::Lines);
					}

					const glm::vec3 v3 = sphere.m_Vertices[sphere.m_NLongitude * i + j + 2] - sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1];
					const glm::vec3 v4 = sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 2] - sphere.m_Vertices[sphere.m_NLongitude * i + j + 2];
					if (glm::cross(v3, v4).z > 0.0f)
					{
						sf::Vertex line[] =
						{
							sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform)),
							sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform)),
							sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform)),
							sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 2], transform)),
							sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 2], transform)),
							sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform))
						};
						window.draw(line, 6, sf::Lines);
					}
				}

				const size_t j = sphere.m_NLongitude - 1;
				const glm::vec3 v1 = sphere.m_Vertices[sphere.m_NLongitude * i + 1] - sphere.m_Vertices[sphere.m_NLongitude * i + j + 1];
				const glm::vec3 v2 = sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1] - sphere.m_Vertices[sphere.m_NLongitude * i + 1];
				if (glm::cross(v1, v2).z > 0.0f)
				{
					sf::Vertex line[] =
					{
						sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[sphere.m_NLongitude * i + j + 1], transform)),
						sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[sphere.m_NLongitude * i + 1], transform)),
						sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[sphere.m_NLongitude * i + 1], transform)),
						sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform)),
						sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform)),
						sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[sphere.m_NLongitude * i + j + 1], transform))
					};
					window.draw(line, 6, sf::Lines);
				}

				const glm::vec3 v3 = sphere.m_Vertices[sphere.m_NLongitude * i + 1] - sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1];
				const glm::vec3 v4 = sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + 1] - sphere.m_Vertices[sphere.m_NLongitude * i + 1];
				if (glm::cross(v3, v4).z > 0.0f)
				{
					sf::Vertex line[] =
					{
						sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform)),
							sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[sphere.m_NLongitude * i + 1], transform)),
							sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[sphere.m_NLongitude * i + 1], transform)),
							sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + 1], transform)),
							sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + 1], transform)),
							sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform))
					};
					window.draw(line, 6, sf::Lines);
				}
			}

			// Top part
			const size_t topMostVertex = sphere.m_NLongitude * (sphere.m_NLatitude * 2 - 1) + 2 - 1;
			for (size_t i = 0; i < sphere.m_NLongitude - 1; ++i)
			{
				const glm::vec3 v1 = sphere.m_Vertices[topMostVertex - sphere.m_NLongitude + i + 1] - sphere.m_Vertices[topMostVertex];
				const glm::vec3 v2 = sphere.m_Vertices[topMostVertex - sphere.m_NLongitude + i] - sphere.m_Vertices[topMostVertex - sphere.m_NLongitude + i + 1];
				if (glm::cross(v1, v2).z > 0.0f)
				{
					sf::Vertex line[] =
					{
						sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[topMostVertex], transform)),
						sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude + i + 1], transform)),
						sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude + i + 1], transform)),
						sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude + i], transform)),
						sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude + i], transform)),
						sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[topMostVertex], transform))
					};
					window.draw(line, 6, sf::Lines);
				}
			}

			const glm::vec3 v3 = sphere.m_Vertices[topMostVertex - sphere.m_NLongitude + 1] - sphere.m_Vertices[topMostVertex];
			const glm::vec3 v4 = sphere.m_Vertices[topMostVertex - 1] - sphere.m_Vertices[topMostVertex - sphere.m_NLongitude + 1];
			if (glm::cross(v3, v4).z > 0.0f)
			{
				sf::Vertex line[] =
				{
					sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[topMostVertex], transform)),
					sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude + 1], transform)),
					sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude + 1], transform)),
					sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[topMostVertex - 1], transform)),
					sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[topMostVertex - 1], transform)),
					sf::Vertex(Normalize3DToProjection(sphere.m_Vertices[topMostVertex], transform))
				};
				window.draw(line, 6, sf::Lines);
			}
	});
}

inline sf::Vector2f RenderSystem::Normalize3DToProjection(const glm::vec4& v, const TransformComponent& transform)
{
	const glm::vec4 result = m_MatTransform * transform.m_MatTransform * v;
	return sf::Vector2f(result.x, result.y);
}
