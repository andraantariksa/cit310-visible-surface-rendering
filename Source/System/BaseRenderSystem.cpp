#include "BaseRenderSystem.hpp"

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <cmath>
#include <cassert>
#include <optional>

#include "../Macro.hpp"
#include "../Component/TransformComponent.hpp"
#include "../Component/Shape3DComponent.hpp"
#include "../Component/SurfaceComponent.hpp"
#include "../Core/Camera.hpp"
#include "../Util/Logger.hpp"

#define PI 3.14159265358979323846f
#define RAD 0.0174533f

BaseRenderSystem::BaseRenderSystem(float vanishingPointZ):
	// Painter as the default because it's the first one I made :p
	m_RenderMode(RenderMethod::Painter)
{
	const float scaleX = 1.0f;
	const float scaleY = 1.0f;
	const float translateX = (float)WINDOW_WIDTH / 2;
	const float translateY = (float)WINDOW_HEIGHT / 2;

	m_MatVCSToSCS = glm::mat4(
		glm::vec4(scaleX, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, -scaleY, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 0.0f),
		glm::vec4(translateX, translateY, 0.0f, 1.0f)
	);

	m_MatWCSToVCS = glm::mat4(
		glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f / vanishingPointZ),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);
}

void BaseRenderSystem::Update(entt::registry& registry)
{
	// Always clear before using it! It will not deallocate the memory
	m_SurfacesVCS.clear();

	registry.view<Shape3DComponent, TransformComponent>().each(
		[&](auto entity, Shape3DComponent& sphere, const TransformComponent& transform)
		{
			for (SurfaceComponent& surface : sphere.m_Surfaces)
			{
				SurfaceComponent surfaceVCS(
					TransformWCSToVCS(TransformOCSToWCS(glm::vec4(surface.m_Vertices[0], 1.0f), transform)),
					TransformWCSToVCS(TransformOCSToWCS(glm::vec4(surface.m_Vertices[1], 1.0f), transform)),
					TransformWCSToVCS(TransformOCSToWCS(glm::vec4(surface.m_Vertices[2], 1.0f), transform)),
					surface.m_Color);
				if (!IsSurfaceIsBackFaceCulled(surfaceVCS))
				{
					m_SurfacesVCS.push_back(surfaceVCS);
				}
			}
		}
	);

	switch (m_RenderMode)
	{
	case BaseRenderSystem::RenderMethod::None:
		break;
	case BaseRenderSystem::RenderMethod::Painter:
		m_SystemPainterRender.Update(registry, *this, m_SurfacesVCS);
		break;
	case BaseRenderSystem::RenderMethod::ZBuffer:
		m_SystemZBufferRender.Update(registry);
		break;
	default:
		break;
	}
}

void BaseRenderSystem::Render(entt::registry& registry, sf::RenderWindow& window)
{
	switch (m_RenderMode)
	{
	case BaseRenderSystem::RenderMethod::None:
		break;
	case BaseRenderSystem::RenderMethod::Painter:
		m_SystemPainterRender.Render(registry, window);
		break;
	case BaseRenderSystem::RenderMethod::ZBuffer:
		m_SystemZBufferRender.Render(registry, window);
		break;
	default:
		break;
	}
}

void BaseRenderSystem::ResetMatrix(float vanishing_point_z)
{
}

bool BaseRenderSystem::IsSurfaceIsBackFaceCulled(const SurfaceComponent& surface)
{
	const glm::vec3 v1 = surface.m_Vertices[1] - surface.m_Vertices[0];
	const glm::vec3 v2 = surface.m_Vertices[2] - surface.m_Vertices[1];
	return glm::dot(glm::cross(v1, v2), m_Camera.m_Direction) >= 0.0f;
}

sf::Vector2f BaseRenderSystem::TransformVec4GLMToVec2SFML(const glm::vec4& v)
{
	const glm::vec2 vertex = v;
	return sf::Vector2f(vertex.x, vertex.y);
}

glm::vec4 BaseRenderSystem::TransformVCSToSCS(const glm::vec4& v)
{
	const auto result = m_MatVCSToSCS * v;
	assert(result.w == 0.0f && "Uh oh w is 0");
	return glm::vec4(result.x / result.w, result.y / result.w, result.z / result.w, 1.0f);
}

glm::vec4 BaseRenderSystem::TransformWCSToVCS(const glm::vec4& v)
{
	return m_MatWCSToVCS * v;
}

glm::vec4 BaseRenderSystem::TransformOCSToWCS(const glm::vec4& v, const TransformComponent& transform)
{
	return transform.m_MatTransform * v;
}
