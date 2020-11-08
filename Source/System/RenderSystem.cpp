#include "RenderSystem.hpp"

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <cmath>
#include <optional>

#include "../Macro.hpp"
#include "../Component/TransformComponent.hpp"
#include "../Component/SphereComponent.hpp"
#include "../Core/Camera.hpp"
#include "../Util/Logger.hpp"

#define PI 3.14159265358979323846f
#define RAD 0.0174533f

RenderSystem::RenderSystem() :
	m_MatTransform(1.0f)
{
	ResetMatrix();
}

// need refactor
void RenderSystem::ResetMatrix(float vanishing_point_z)
{
	const float scaleX = 1.0f;
	const float scaleY = 1.0f;
	const float translateX = (float)WINDOW_WIDTH / 2;
	const float translateY = (float)WINDOW_HEIGHT / 2;

	// St
	m_MatVCSToSCS = glm::mat4(
		glm::vec4(scaleX, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, -scaleY, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
		glm::vec4(translateX, translateY, 0.0f, 1.0f)
	);

	// Perspective Projection
	// Vt
	m_MatWCSToVCS = glm::mat4(
		glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f / vanishing_point_z),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);

	m_MatTransform = m_MatVCSToSCS * m_MatWCSToVCS;
}

void RenderSystem::ResetTransform(entt::registry& registry)
{
	ResetMatrix();
	registry.view<SphereComponent, TransformComponent>().each([&](auto entity, SphereComponent& sphere, TransformComponent& transform)
		{
			transform.ResetMatrix();
		});
}

bool once = true;

void RenderSystem::Render(entt::registry& registry, sf::RenderWindow& window)
{
	registry.view<SphereComponent, TransformComponent>().each([&](auto entity, SphereComponent& sphere, const TransformComponent& transform)
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

			// Toggle backface cull
			if (sphere.m_IsCullBackface)
			{
				// In case you want to use surface
				//sf::ConvexShape convex(3);
				//convex.setFillColor(sf::Color::Green);
				//convex.setOutlineColor(sf::Color::Green);
				//convex.setOutlineThickness(1.0f);

				// Bottom part
				for (size_t i = 0; i < sphere.m_NLongitude - 1; ++i)
				{
					const glm::vec3 v1 = TransformToVCSVec4(sphere.m_Vertices[0], transform) - TransformToVCSVec4(sphere.m_Vertices[i + 1], transform);
					const glm::vec3 v2 = TransformToVCSVec4(sphere.m_Vertices[i + 2], transform) - TransformToVCSVec4(sphere.m_Vertices[0], transform);
					if (glm::dot(glm::cross(v1, v2), m_Camera.m_Direction) < 0.0f)
					{
						sf::Vertex line[] =
						{
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[i + 1], transform)),
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[0], transform)),
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[0], transform)),
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[i + 2], transform)),
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[i + 2], transform)),
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[i + 1], transform))
						};
						window.draw(line, 6, sf::Lines);

						//		// In case you want to use surface
						//		//convex.setPoint(0, Normalize3DToProjectionSFML(m_EntitySphere.m_Vertices[0], transform));
						//		//convex.setPoint(1, Normalize3DToProjectionSFML(m_EntitySphere.m_Vertices[i + 1], transform));
						//		//convex.setPoint(2, Normalize3DToProjectionSFML(m_EntitySphere.m_Vertices[i + 2], transform));
						//		//window.draw(convex);
					}
				}


				const glm::vec3 v1 = TransformToVCSVec4(sphere.m_Vertices[0], transform) - TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude], transform);
				const glm::vec3 v2 = TransformToVCSVec4(sphere.m_Vertices[1], transform) - TransformToVCSVec4(sphere.m_Vertices[0], transform);
				if (glm::dot(glm::cross(v1, v2), m_Camera.m_Direction) < 0.0f)
				{
					sf::Vertex line[] =
					{
						sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude], transform)),
						sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[0], transform)),
						sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[0], transform)),
						sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[1], transform)),
						sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[1], transform)),
						sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude], transform))
					};
					window.draw(line, 6, sf::Lines);
				}

				// Middle part
				for (size_t i = 1; i < sphere.m_NLatitude * 2 - 1; ++i)
				{
					for (size_t j = 0; j < sphere.m_NLongitude - 1; ++j)
					{
						const glm::vec3 v1 = TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform) - TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform);
						const glm::vec3 v2 = TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + j + 1], transform) - TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform);
						if (glm::dot(glm::cross(v1, v2), m_Camera.m_Direction) < 0.0f)
						{
							sf::Vertex line[] =
							{
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * i + j + 1], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * i + j + 1], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform))
							};
							window.draw(line, 6, sf::Lines);
						}

						const glm::vec3 v3 = TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform) - TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 2], transform);
						const glm::vec3 v4 = TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform) - TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform);
						if (glm::dot(glm::cross(v3, v4), m_Camera.m_Direction) < 0.0f)
						{
							sf::Vertex line[] =
							{
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 2], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 2], transform))
							};
							window.draw(line, 6, sf::Lines);
						}
					}

					const size_t j = sphere.m_NLongitude - 1;
					const glm::vec3 v1 = TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + 1], transform) - TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform);
					const glm::vec3 v2 = TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + j + 1], transform) - TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + 1], transform);
					if (glm::dot(glm::cross(v1, v2), m_Camera.m_Direction) < 0.0f)
					{
						sf::Vertex line[] =
						{
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform)),
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * i + 1], transform)),
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * i + 1], transform)),
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * i + j + 1], transform)),
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * i + j + 1], transform)),
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform))
						};
						window.draw(line, 6, sf::Lines);
					}

					const glm::vec3 v3 = TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + 1], transform) - TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + 1], transform);
					const glm::vec3 v4 = TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform) - TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + 1], transform);
					if (glm::dot(glm::cross(v3, v4), m_Camera.m_Direction) < 0.0f)
					{
						sf::Vertex line[] =
						{
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + 1], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * i + 1], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * i + 1], transform)),
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + 1], transform))
						};
						window.draw(line, 6, sf::Lines);
					}
				}

				// Top part
				const size_t topMostVertex = sphere.m_Vertices.size() - 1;
				for (size_t i = 0; i < sphere.m_NLongitude - 1; ++i)
				{
					const glm::vec3 v1 = TransformToVCSVec4(sphere.m_Vertices[topMostVertex], transform) - TransformToVCSVec4(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude + i + 1], transform);
					const glm::vec3 v2 = TransformToVCSVec4(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude + i], transform) - TransformToVCSVec4(sphere.m_Vertices[topMostVertex], transform);
					if (glm::dot(glm::cross(v1, v2), m_Camera.m_Direction) < 0.0f)
					{
						sf::Vertex line[] =
						{
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude + i + 1], transform)),
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[topMostVertex], transform)),
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[topMostVertex], transform)),
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude + i], transform)),
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude + i], transform)),
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude + i + 1], transform))
						};
						window.draw(line, 6, sf::Lines);
					}
				}

				const glm::vec3 v3 = TransformToVCSVec4(sphere.m_Vertices[topMostVertex], transform) - TransformToVCSVec4(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude], transform);
				const glm::vec3 v4 = TransformToVCSVec4(sphere.m_Vertices[topMostVertex - 1], transform) - TransformToVCSVec4(sphere.m_Vertices[topMostVertex], transform);
				if (glm::dot(glm::cross(v3, v4), m_Camera.m_Direction) < 0.0f)
				{
					sf::Vertex line[] =
					{
						sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude], transform)),
						sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[topMostVertex], transform)),
						sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[topMostVertex], transform)),
						sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[topMostVertex - 1], transform)),
						sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[topMostVertex - 1], transform)),
						sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude], transform))
					};
					window.draw(line, 6, sf::Lines);
				}
			}
			else
			{
				// Bottom part
				for (size_t i = 0; i < sphere.m_NLongitude - 1; ++i)
				{
					sf::Vertex line[] =
					{
						sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[i + 1], transform)),
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[0], transform)),
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[0], transform)),
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[i + 2], transform)),
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[i + 2], transform)),
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[i + 1], transform))
					};
					window.draw(line, 6, sf::Lines);

					// In case you want to use surface
					//convex.setPoint(0, Normalize3DToProjection(m_EntitySphere.m_Vertices[0], transform));
					//convex.setPoint(1, Normalize3DToProjection(m_EntitySphere.m_Vertices[i + 1], transform));
					//convex.setPoint(2, Normalize3DToProjection(m_EntitySphere.m_Vertices[i + 2], transform));
					//window.draw(convex);
				}

				sf::Vertex line[] =
				{
					sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude], transform)),
						sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[0], transform)),
						sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[0], transform)),
						sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[1], transform)),
						sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[1], transform)),
						sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude], transform))
				};
				window.draw(line, 6, sf::Lines);

				// Middle part
				for (size_t i = 1; i < sphere.m_NLatitude * 2 - 1; ++i)
				{
					for (size_t j = 0; j < sphere.m_NLongitude - 1; ++j)
					{
						sf::Vertex line[] =
						{
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * i + j + 1], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * i + j + 1], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 2], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 2], transform))
						};
						window.draw(line, 12, sf::Lines);
					}

					const size_t j = sphere.m_NLongitude - 1;
					sf::Vertex line[] =
					{
						sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 2], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 2], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + 1], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * i + 1], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * i + 1], transform)),
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform)),
								sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + 1], transform))
					};
					window.draw(line, 12, sf::Lines);
				}

				// Top part
				const size_t topMostVertex = sphere.m_Vertices.size() - 1;
				for (size_t i = 0; i < sphere.m_NLongitude - 1; ++i)
				{
					sf::Vertex line[] =
					{
						sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude + i + 1], transform)),
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[topMostVertex], transform)),
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[topMostVertex], transform)),
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude + i], transform)),
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude + i], transform)),
							sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude + i + 1], transform))
					};
					window.draw(line, 6, sf::Lines);
				}

				sf::Vertex line2[] =
				{
						sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude], transform)),
						sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[topMostVertex], transform)),
						sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[topMostVertex], transform)),
						sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[topMostVertex - 1], transform)),
						sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[topMostVertex - 1], transform)),
						sf::Vertex(TransformToSCSVec2SFML(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude], transform))
				};
				window.draw(line, 6, sf::Lines);
			}
		});
}

inline sf::Vector2f RenderSystem::TransformToSCSVec2SFML(const glm::vec4& v, const TransformComponent& transform)
{
	const glm::vec4& result = TransformToSCS(v, transform);
	return sf::Vector2f(result.x / result.w, result.y / result.w);
}

inline glm::vec2 RenderSystem::TransformToSCSVec2GLM(const glm::vec4& v, const TransformComponent& transform)
{
	const glm::vec4& result = TransformToSCS(v, transform);
	return glm::vec2(result.x / result.w, result.y / result.w);
}

inline glm::vec4 RenderSystem::TransformToWCSVec4(const glm::vec4& v, const TransformComponent& transform)
{
	const glm::vec4& result = TransformToWCS(v, transform);
	return glm::vec4(result.x / result.w, result.y / result.w, result.z / result.w, 1.0f);
}

inline glm::vec4 RenderSystem::TransformToVCSVec4(const glm::vec4& v, const TransformComponent& transform)
{
	const glm::vec4& result = TransformToVCS(v, transform);
	return glm::vec4(result.x / result.w, result.y / result.w, result.z / result.w, 1.0f);
}

inline glm::vec4 RenderSystem::TransformToWCS(const glm::vec4& v, const TransformComponent& transform)
{
	return transform.m_MatTransform * v;
}

inline glm::vec4 RenderSystem::TransformToVCS(const glm::vec4& v, const TransformComponent& transform)
{
	return m_MatWCSToVCS * TransformToWCS(v, transform);
}

inline glm::vec4 RenderSystem::TransformToSCS(const glm::vec4& v, const TransformComponent& transform)
{
	return m_MatTransform * TransformToWCS(v, transform);
}

// Bonus Implementation
//https://stackoverflow.com/a/2049593/3894179
static float SignPolygonSide(glm::vec2& pointToCheck, glm::vec2& p1, glm::vec2& p2)
{
	return glm::dot(glm::vec2(p2.y - p1.y, -(p2.x - p1.x)), glm::vec2(pointToCheck.x - p1.x, pointToCheck.y - p1.y));
}

// Check whether point is inside triangle {v1, v2, v3} in 2D
static bool IsInsideTriangle2D(glm::vec2& point, glm::vec2& v1, glm::vec2& v2, glm::vec2& v3)
{
	float d1 = SignPolygonSide(point, v1, v2);
	float d2 = SignPolygonSide(point, v2, v3);
	float d3 = SignPolygonSide(point, v3, v1);
	return (d1 > 0.0f) && (d2 > 0.0f) && (d3 > 0.0f);
}

std::optional<glm::vec3> RenderSystem::GetSphereSurfaceNormal(entt::registry& registry, glm::vec2& click_coord)
{
	std::optional<glm::vec3> normal_return;
	registry.view<SphereComponent, TransformComponent>().each([&](auto entity, SphereComponent& sphere, const TransformComponent& transform)
		{
			// Bottom part
			for (size_t i = 0; i < sphere.m_NLongitude - 1; ++i)
			{
				const glm::vec3 v1 = TransformToVCSVec4(sphere.m_Vertices[0], transform) - TransformToVCSVec4(sphere.m_Vertices[i + 1], transform);
				const glm::vec3 v2 = TransformToVCSVec4(sphere.m_Vertices[i + 2], transform) - TransformToVCSVec4(sphere.m_Vertices[0], transform);
				if (glm::dot(glm::cross(v1, v2), m_Camera.m_Direction) < 0.0f)
				{
					if (IsInsideTriangle2D(
						click_coord,
						TransformToSCSVec2GLM(sphere.m_Vertices[i + 1], transform),
						TransformToSCSVec2GLM(sphere.m_Vertices[0], transform),
						TransformToSCSVec2GLM(sphere.m_Vertices[i + 2], transform)
					))
					{
						const glm::vec3 v_a = TransformToWCSVec4(sphere.m_Vertices[0], transform) - TransformToWCSVec4(sphere.m_Vertices[i + 1], transform);
						const glm::vec3 v_b = TransformToWCSVec4(sphere.m_Vertices[i + 2], transform) - TransformToWCSVec4(sphere.m_Vertices[0], transform);
						normal_return = glm::cross(v_a, v_b);
						return;
					}
				}
			}

			const glm::vec3 v1 = TransformToVCSVec4(sphere.m_Vertices[0], transform) - TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude], transform);
			const glm::vec3 v2 = TransformToVCSVec4(sphere.m_Vertices[1], transform) - TransformToVCSVec4(sphere.m_Vertices[0], transform);
			if (glm::dot(glm::cross(v1, v2), m_Camera.m_Direction) < 0.0f)
			{
				if (IsInsideTriangle2D(click_coord,
					TransformToSCSVec2GLM(sphere.m_Vertices[sphere.m_NLongitude], transform),
					TransformToSCSVec2GLM(sphere.m_Vertices[0], transform),
					TransformToSCSVec2GLM(sphere.m_Vertices[1], transform)
				))
				{
					const glm::vec3 v_a = TransformToWCSVec4(sphere.m_Vertices[0], transform) - TransformToWCSVec4(sphere.m_Vertices[sphere.m_NLongitude], transform);
					const glm::vec3 v_b = TransformToWCSVec4(sphere.m_Vertices[1], transform) - TransformToWCSVec4(sphere.m_Vertices[0], transform);
					normal_return = glm::cross(v_a, v_b);
					return;
				}
			}

			//// Middle part
			for (size_t i = 1; i < sphere.m_NLatitude * 2 - 1; ++i)
			{
				for (size_t j = 0; j < sphere.m_NLongitude - 1; ++j)
				{
					const glm::vec3 v1 = TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform) - TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform);
					const glm::vec3 v2 = TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + j + 1], transform) - TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform);
					if (glm::dot(glm::cross(v1, v2), m_Camera.m_Direction) < 0.0f)
					{
						if (IsInsideTriangle2D(click_coord,
							TransformToSCSVec2GLM(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform),
							TransformToSCSVec2GLM(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform),
							TransformToSCSVec2GLM(sphere.m_Vertices[sphere.m_NLongitude * i + j + 1], transform)
						))
						{
							const glm::vec3 v_a = TransformToWCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform) - TransformToWCSVec4(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform);
							const glm::vec3 v_b = TransformToWCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + j + 1], transform) - TransformToWCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform);
							normal_return = glm::cross(v_a, v_b);
							return;
						}
					}

					const glm::vec3 v3 = TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform) - TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 2], transform);
					const glm::vec3 v4 = TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform) - TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform);
					if (glm::dot(glm::cross(v3, v4), m_Camera.m_Direction) < 0.0f)
					{
						if (IsInsideTriangle2D(click_coord,
							TransformToSCSVec2GLM(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 2], transform),
							TransformToSCSVec2GLM(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform),
							TransformToSCSVec2GLM(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform)
						))
						{
							const glm::vec3 v_a = TransformToWCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform) - TransformToWCSVec4(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 2], transform);
							const glm::vec3 v_b = TransformToWCSVec4(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform) - TransformToWCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + j + 2], transform);
							normal_return = glm::cross(v_a, v_b);
							return;
						}
					}
				}

				const size_t j = sphere.m_NLatitude - 1;
				const glm::vec3 v1 = TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + 1], transform) - TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform);
				const glm::vec3 v2 = TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + j + 1], transform) - TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + 1], transform);
				if (glm::dot(glm::cross(v1, v2), m_Camera.m_Direction) < 0.0f)
				{
					if (IsInsideTriangle2D(click_coord,
						TransformToSCSVec2GLM(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform),
						TransformToSCSVec2GLM(sphere.m_Vertices[sphere.m_NLongitude * i + 1], transform),
						TransformToSCSVec2GLM(sphere.m_Vertices[sphere.m_NLongitude * i + j + 1], transform)
					))
					{
						const glm::vec3 v_a = TransformToWCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + 1], transform) - TransformToWCSVec4(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform);
						const glm::vec3 v_b = TransformToWCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + j + 1], transform) - TransformToWCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + 1], transform);
						normal_return = glm::cross(v_a, v_b);
						return;
					}
				}

				const glm::vec3 v3 = TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + 1], transform) - TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + 1], transform);
				const glm::vec3 v4 = TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform) - TransformToVCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + 1], transform);
				if (glm::dot(glm::cross(v3, v4), m_Camera.m_Direction) < 0.0f)
				{
					if (IsInsideTriangle2D(click_coord,
						TransformToSCSVec2GLM(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + 1], transform),
						TransformToSCSVec2GLM(sphere.m_Vertices[sphere.m_NLongitude * i + 1], transform),
						TransformToSCSVec2GLM(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform)
					))
					{
						const glm::vec3 v_a = TransformToWCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + 1], transform) - TransformToWCSVec4(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + 1], transform);
						const glm::vec3 v_b = TransformToWCSVec4(sphere.m_Vertices[sphere.m_NLongitude * (i - 1) + j + 1], transform) - TransformToWCSVec4(sphere.m_Vertices[sphere.m_NLongitude * i + 1], transform);
						normal_return = glm::cross(v_a, v_b);
						return;
					}
				}
			}

			//// Top part
			const size_t topMostVertex = sphere.m_Vertices.size() - 1;
			for (size_t i = 0; i < sphere.m_NLongitude - 1; ++i)
			{
				const glm::vec3 v1 = TransformToVCSVec4(sphere.m_Vertices[topMostVertex], transform) - TransformToVCSVec4(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude + i + 1], transform);
				const glm::vec3 v2 = TransformToVCSVec4(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude + i], transform) - TransformToVCSVec4(sphere.m_Vertices[topMostVertex], transform);
				if (glm::dot(glm::cross(v1, v2), m_Camera.m_Direction) < 0.0f)
				{
					if (IsInsideTriangle2D(click_coord,
						TransformToSCSVec2GLM(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude + i + 1], transform),
						TransformToSCSVec2GLM(sphere.m_Vertices[topMostVertex], transform),
						TransformToSCSVec2GLM(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude + i], transform)
					))
					{
						const glm::vec3 v_a = TransformToWCSVec4(sphere.m_Vertices[topMostVertex], transform) - TransformToWCSVec4(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude + i + 1], transform);
						const glm::vec3 v_b = TransformToWCSVec4(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude + i], transform) - TransformToWCSVec4(sphere.m_Vertices[topMostVertex], transform);
						normal_return = glm::cross(v_a, v_b);
						return;
					}
				}
			}

			const glm::vec3 v3 = TransformToVCSVec4(sphere.m_Vertices[topMostVertex], transform) - TransformToVCSVec4(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude], transform);
			const glm::vec3 v4 = TransformToVCSVec4(sphere.m_Vertices[topMostVertex - 1], transform) - TransformToVCSVec4(sphere.m_Vertices[topMostVertex], transform);
			if (glm::dot(glm::cross(v3, v4), m_Camera.m_Direction) < 0.0f)
			{
				if (IsInsideTriangle2D(click_coord,
					TransformToSCSVec2GLM(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude], transform),
					TransformToSCSVec2GLM(sphere.m_Vertices[topMostVertex], transform),
					TransformToSCSVec2GLM(sphere.m_Vertices[topMostVertex - 1], transform)
				))
				{
					const glm::vec3 v_a = TransformToWCSVec4(sphere.m_Vertices[topMostVertex], transform) - TransformToWCSVec4(sphere.m_Vertices[topMostVertex - sphere.m_NLongitude], transform);
					const glm::vec3 v_b = TransformToWCSVec4(sphere.m_Vertices[topMostVertex - 1], transform) - TransformToWCSVec4(sphere.m_Vertices[topMostVertex], transform);
					normal_return = glm::cross(v_a, v_b);
					return;
				}
			}
		});
	return normal_return;
}
