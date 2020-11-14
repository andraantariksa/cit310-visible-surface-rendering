#include "PainterRenderSystem.hpp"

#include <glm/glm.hpp>

#include "BaseRenderSystem.hpp"
#include "../Component/TransformComponent.hpp"
#include "../Component/Shape3DComponent.hpp"
#include "../Component/SurfaceComponent.hpp"
#include "../Component/Surface2DComponent.hpp"
#include "../Misc/BinaryTreePartitioning.hpp"
#include "../Util/Logger.hpp"

void PainterRenderSystem::Update(entt::registry& registry, BaseRenderSystem& baseRenderSystem, std::vector<SurfaceComponent>& surfacesVCS)
{
	m_CachedSurfaces2D.clear();

	BinaryTreePartitioning bsp(surfacesVCS);

	std::function<void(std::vector<SurfaceComponent>&)> traverseLambda = [&](std::vector<SurfaceComponent>& surfaces) -> void {
		for (auto& surface : surfaces)
		{
			m_CachedSurfaces2D.push_back(
				Surface2DComponent(
					baseRenderSystem.TransformVec4GLMToVec2SFML(baseRenderSystem.TransformVCSToSCS(glm::vec4(surface.m_Vertices[0], 1.0f))),
					baseRenderSystem.TransformVec4GLMToVec2SFML(baseRenderSystem.TransformVCSToSCS(glm::vec4(surface.m_Vertices[1], 1.0f))),
					baseRenderSystem.TransformVec4GLMToVec2SFML(baseRenderSystem.TransformVCSToSCS(glm::vec4(surface.m_Vertices[2], 1.0f))),
					surface.m_Color)
			);
		}
	};

	bsp.Traverse(traverseLambda);
}

void PainterRenderSystem::Render(entt::registry& registry, sf::RenderWindow& window)
{
	for (auto& surface2D : m_CachedSurfaces2D)
	{
		DrawSurface(window, surface2D);
	}
}

void PainterRenderSystem::DrawSurface(sf::RenderWindow& window, const Surface2DComponent& surface)
{
	sf::VertexArray triangle(sf::Triangles, 3);
	triangle[0].position = surface.m_Vertices[0];
	triangle[1].position = surface.m_Vertices[1];
	triangle[2].position = surface.m_Vertices[2];
	triangle[0].color = surface.m_Color;
	triangle[1].color = surface.m_Color;
	triangle[2].color = surface.m_Color;
	window.draw(triangle);
}
