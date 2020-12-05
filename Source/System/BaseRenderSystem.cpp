#include "BaseRenderSystem.hpp"

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <iostream>
#include <glm/glm.hpp>
#include <cmath>
#ifdef _DEBUG_BUILD
	#include <cassert>
#else
	#include <stdexcept>
#endif
#include <optional>

#include "../Macro.hpp"
#include "../Component/TransformComponent.hpp"
#include "../Component/Shape3DComponent.hpp"
#include "../Component/Surface3DComponent.hpp"
#include "../Component/Triangle3DComponent.hpp"
#include "../Core/Camera.hpp"
#include "../Util/Logger.hpp"

#define PI 3.14159265358979323846f
#define RAD 0.0174533f

BaseRenderSystem::BaseRenderSystem(RenderMethod renderMethod, double vanishingPointZ):
	// Painter as the default because it's the first one I made :p
	m_RenderMethod(renderMethod)
{
	m_Texture.create(WINDOW_WIDTH, WINDOW_HEIGHT);
	m_Sprite.setTexture(m_Texture);
	TextureClear();

	const double scaleX = 1.0;
	const double scaleY = 1.0;
	const double translateX = (double)WINDOW_WIDTH / 2;
	const double translateY = (double)WINDOW_HEIGHT / 2;

	m_MatVCSToSCS = glm::dmat4(
		glm::dvec4(scaleX, 0.0, 0.0, 0.0),
		glm::dvec4(0.0, -scaleY, 0.0, 0.0),
		glm::dvec4(0.0, 0.0, 1.0, 0.0),
		glm::dvec4(translateX, translateY, 0.0, 1.0)
	);

	m_MatWCSToVCS = glm::dmat4(
		glm::dvec4(1.0, 0.0, 0.0, 0.0),
		glm::dvec4(0.0, 1.0, 0.0, 0.0),
		glm::dvec4(0.0, 0.0, 1.0, 1.0 / -vanishingPointZ),
		glm::dvec4(0.0, 0.0, 0.0, 1.0)
	);

	m_MatTransform = m_MatVCSToSCS * m_MatWCSToVCS;
}

void BaseRenderSystem::Update(entt::registry& registry)
{
	// Always clear before using it! It will not deallocate the memory
	m_Shapes3DVCS.clear();

	registry.view<Shape3DComponent, TransformComponent>().each(
		[&](auto entity, Shape3DComponent& shape3D, const TransformComponent& transform)
		{
			// Transform the vertices
			Shape3DComponent shape3DVCS;
			shape3DVCS.m_Vertices = shape3D.m_Vertices;
			for (std::size_t i = 0; i < shape3D.m_Vertices.size(); ++i)
			{
				shape3DVCS.m_Vertices[i] =
					TransformWCSToVCS(
						TransformOCSToWCS(
							glm::dvec4(shape3D.m_Vertices[i], 1.0),
							transform
						)
					);
			}

			for (Surface3DComponent& surface : shape3D.m_Surfaces)
			{
				if (!IsSurfaceIsBackFaceCulled(
					std::array<glm::dvec3, 3>({
						shape3DVCS.m_Vertices[surface.m_VertexIndices[0]],
						shape3DVCS.m_Vertices[surface.m_VertexIndices[1]],
						shape3DVCS.m_Vertices[surface.m_VertexIndices[2]] })
				))
				{
					shape3DVCS.m_Surfaces.push_back(
						Surface3DComponent(
							surface.m_VertexIndices,
							surface.m_Color
						)
					);
				}
			}
			m_Shapes3DVCS.push_back(std::move(shape3DVCS));
		}
	);

	std::vector<Triangle3DComponent> preprocessedTriangles;

	switch (m_RenderMethod)
	{
	case BaseRenderSystem::RenderMethod::None:
		break;
	case BaseRenderSystem::RenderMethod::Painter:
		for (Shape3DComponent& shape3D : m_Shapes3DVCS)
		{
			for (Surface3DComponent& surface3D : shape3D.m_Surfaces)
			{
				preprocessedTriangles.push_back(
					std::move(Triangle3DComponent(
						shape3D.m_Vertices[surface3D.m_VertexIndices[0]],
						shape3D.m_Vertices[surface3D.m_VertexIndices[1]],
						shape3D.m_Vertices[surface3D.m_VertexIndices[2]],
						surface3D.m_Color
					))
				);
			}
		}
		m_SystemPainterRender.Update(registry, *this, preprocessedTriangles);
		break;
	case BaseRenderSystem::RenderMethod::ZBuffer:
		for (Shape3DComponent& shape3D : m_Shapes3DVCS)
		{
			for (Surface3DComponent& surface3D : shape3D.m_Surfaces)
			{
				preprocessedTriangles.push_back(
					// Change it into SCS and preserve the Z value, which will used by the Z buffer
					std::move(Triangle3DComponent(
						TransformVCSToSCS(glm::dvec4(shape3D.m_Vertices[surface3D.m_VertexIndices[0]], 1.0)),
						TransformVCSToSCS(glm::dvec4(shape3D.m_Vertices[surface3D.m_VertexIndices[1]], 1.0)),
						TransformVCSToSCS(glm::dvec4(shape3D.m_Vertices[surface3D.m_VertexIndices[2]], 1.0)),
						surface3D.m_Color
					))
				);
			}
		}
		m_SystemZBufferRender.Update(registry, *this, preprocessedTriangles);
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
	Update(registry);
}

void BaseRenderSystem::ResetMatrix(double vanishing_point_z)
{
}

bool BaseRenderSystem::IsSurfaceIsBackFaceCulled(const std::array<glm::dvec3, 3>& triangle)
{
	const glm::dvec3 v1 = triangle[1] - triangle[0];
	const glm::dvec3 v2 = triangle[2] - triangle[1];
	return glm::dot(glm::cross(v1, v2), Camera::Direction) >= 0.0;
}

sf::Vector2f BaseRenderSystem::TransformVec4GLMToVec2SFML(const glm::dvec4& v)
{
#ifdef _DEBUG_BUILD
	assert(v.w != 0.0 && "Uh oh w is 0");
#else
	if (v.w == 0.0)
	{
		throw std::runtime_error("w is 0. Error on BaseRenderSystem::TransformVec4GLMToVec2SFML");
	}
#endif
	return sf::Vector2f((float)(v.x / v.w), (float)(v.y / v.w));
}

glm::dvec4 BaseRenderSystem::TransformVCSToSCS(const glm::dvec4& v)
{
	const auto result = m_MatVCSToSCS * v;
#ifdef _DEBUG_BUILD
	assert(result.w != 0.0 && "Uh oh w is 0");
#else
	if (result.w == 0.0)
	{
		throw std::runtime_error("w is 0. Error in BaseRenderSystem::TransformVCSToSCS");
	}
#endif
	return glm::dvec4(result.x / result.w, result.y / result.w, result.z / result.w, 1.0);
}

glm::dvec4 BaseRenderSystem::TransformWCSToVCS(const glm::dvec4& v)
{
	const auto result = m_MatWCSToVCS * v;
#ifdef _DEBUG_BUILD
	assert(result.w != 0.0 && "Uh oh w is 0");
#else
	if (result.w == 0.0)
	{
		throw std::runtime_error("w is 0. Error in BaseRenderSystem::TransformWCSToVCS");
	}
#endif
	const auto x = result.x / result.w;
	const auto y = result.y / result.w;
#ifdef _DEBUG_BUILD
	assert(x == x && "Uh oh infinity");
#else
	if (x != x)
	{
		throw std::runtime_error("x is inf. Error in BaseRenderSystem::TransformWCSToVCS");
	}
#endif
#ifdef _DEBUG_BUILD
	assert(y == y && "Uh oh infinity");
#else
	if (y != y)
	{
		throw std::runtime_error("y is inf. Error in BaseRenderSystem::TransformWCSToVCS");
	}
#endif
	return glm::dvec4(result.x / result.w, result.y / result.w, result.z / result.w, 1.0);
}

glm::dvec4 BaseRenderSystem::TransformWCSToSCS(const glm::dvec4& v)
{
	return TransformVCSToSCS(TransformWCSToVCS(v));
}

glm::dvec4 BaseRenderSystem::TransformOCSToWCS(const glm::dvec4& v, const TransformComponent& transform)
{
	const auto result = transform.m_MatTransform * v;
	assert(result.w != 0.0 && "Uh oh w is 0");
	return glm::dvec4(result.x / result.w, result.y / result.w, result.z / result.w, 1.0);
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

