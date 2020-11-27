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

BaseRenderSystem::BaseRenderSystem(RenderMethod renderMethod, float vanishingPointZ):
	// Painter as the default because it's the first one I made :p
	m_RenderMethod(renderMethod)
{
	m_Texture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
	m_Sprite.setTexture(m_Texture);
	TextureClear();

	const float scaleX = 1.0f;
	const float scaleY = 1.0f;
	const float translateX = (float)WINDOW_WIDTH / 2;
	const float translateY = (float)WINDOW_HEIGHT / 2;

	m_MatVCSToSCS = glm::mat4(
		glm::vec4(scaleX, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, -scaleY, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
		glm::vec4(translateX, translateY, 0.0f, 1.0f)
	);

	m_MatWCSToVCS = glm::mat4(
		glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 1.0f / -vanishingPointZ),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);

	m_MatTransform = m_MatVCSToSCS * m_MatWCSToVCS;
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

	switch (m_RenderMethod)
	{
	case BaseRenderSystem::RenderMethod::None:
		break;
	case BaseRenderSystem::RenderMethod::Painter:
		m_SystemPainterRender.Update(registry, *this, m_SurfacesVCS);
		break;
	case BaseRenderSystem::RenderMethod::ZBuffer:
		m_SystemZBufferRender.Update(registry, *this, m_SurfacesVCS);
		break;
	default:
		break;
	}
}

void BaseRenderSystem::Render(entt::registry& registry, sf::RenderWindow& window)
{
	switch (m_RenderMethod)
	{
	case BaseRenderSystem::RenderMethod::None:
		break;
	case BaseRenderSystem::RenderMethod::Painter:
		m_SystemPainterRender.Render(registry, *this, window);
		break;
	case BaseRenderSystem::RenderMethod::ZBuffer:
		m_SystemZBufferRender.Render(registry, *this, window);
		break;
	default:
		break;
	}
}

void BaseRenderSystem::ChangeRenderMethod(entt::registry& registry, RenderMethod renderMethod)
{
	m_RenderMethod = renderMethod;
	INFO << (int)m_RenderMethod << '\n';
	Update(registry);
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
	assert(v.w != 0.0f && "Uh oh w is 0");
	return sf::Vector2f(v.x / v.w, v.y / v.w);
}

glm::vec4 BaseRenderSystem::TransformVCSToSCS(const glm::vec4& v)
{
	const auto result = m_MatVCSToSCS * v;
	assert(result.w != 0.0f && "Uh oh w is 0");
	return glm::vec4(result.x / result.w, result.y / result.w, result.z / result.w, 1.0f);
}

glm::vec4 BaseRenderSystem::TransformWCSToVCS(const glm::vec4& v)
{
	const auto result = m_MatWCSToVCS * v;
	assert(result.w != 0.0f && "Uh oh w is 0");
	const auto x = result.x / result.w;
	const auto y = result.y / result.w;
	assert(x == x && "Uh oh infinity");
	assert(y == y && "Uh oh infinity");
	return glm::vec4(result.x / result.w, result.y / result.w, result.z / result.w, 1.0f);
}

glm::vec4 BaseRenderSystem::TransformWCSToSCS(const glm::vec4& v)
{
	return TransformVCSToSCS(TransformWCSToVCS(v));
}

glm::vec4 BaseRenderSystem::TransformOCSToWCS(const glm::vec4& v, const TransformComponent& transform)
{
	const auto result = transform.m_MatTransform * v;
	assert(result.w != 0.0f && "Uh oh w is 0");
	return glm::vec4(result.x / result.w, result.y / result.w, result.z / result.w, 1.0f);
}


void BaseRenderSystem::TextureClear()
{
	sf::Color clearColor(CLEAR_COLOR);
	for (size_t currentPixelPos = 0; currentPixelPos < WINDOW_WIDTH * WINDOW_HEIGHT * 4; currentPixelPos += 4)
	{
		m_TexturePixels[currentPixelPos] = clearColor.r;
		m_TexturePixels[currentPixelPos + 1] = clearColor.g;
		m_TexturePixels[currentPixelPos + 2] = clearColor.b;
		m_TexturePixels[currentPixelPos + 3] = clearColor.a;
	}
}

void BaseRenderSystem::TextureSetPixel(const glm::ivec2& position, const sf::Color& color)
{
	if (position.x >= WINDOW_WIDTH || position.y >= WINDOW_HEIGHT ||
		position.x < 0 || position.y < 0)
	{
		return;
	}

	const size_t currentPixelPos = ((size_t)position.x + ((size_t)position.y * WINDOW_WIDTH)) * 4L;
	m_TexturePixels[currentPixelPos] = color.r;
	m_TexturePixels[currentPixelPos + 1L] = color.g;
	m_TexturePixels[currentPixelPos + 2L] = color.b;
	m_TexturePixels[currentPixelPos + 3L] = color.a;
}

void BaseRenderSystem::Print()
{
	switch (m_RenderMethod)
	{
	case BaseRenderSystem::RenderMethod::None:
		break;
	case BaseRenderSystem::RenderMethod::Painter:
		m_SystemPainterRender.PrintBinaryPartitioningTree();
		break;
	case BaseRenderSystem::RenderMethod::ZBuffer:
		break;
	default:
		break;
	}
}

