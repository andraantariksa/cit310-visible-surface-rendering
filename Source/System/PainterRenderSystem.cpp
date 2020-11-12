#include "PainterRenderSystem.hpp"

#include <glm/glm.hpp>

#include "BaseRenderSystem.hpp"
#include "../Component/TransformComponent.hpp"
#include "../Component/Shape3DComponent.hpp"
#include "../Component/SurfaceComponent.hpp"
#include "../Component/Surface2DComponent.hpp"
#include "../Misc/BinaryTreePartitioning.hpp"

void PainterRenderSystem::Update(entt::registry& registry, BaseRenderSystem& baseRenderSystem, std::vector<SurfaceComponent>& surfacesVCS)
{
	m_CachedSurfaces2D.clear();

	BinaryTreePartitioning bsp(surfacesVCS);

	std::function<void(std::vector<SurfaceComponent>&)> traverseLambda = [&](std::vector<SurfaceComponent>& surfaces) -> void {
		for (auto& it = surfaces.cbegin(); it != surfaces.cend(); ++it)
		{
			m_CachedSurfaces2D.push_back(
				Surface2DComponent(
					baseRenderSystem.TransformVec4GLMToVec2SFML(baseRenderSystem.TransformVCSToSCS(glm::vec4((*it).m_Vertices[0], 1.0f))),
					baseRenderSystem.TransformVec4GLMToVec2SFML(baseRenderSystem.TransformVCSToSCS(glm::vec4((*it).m_Vertices[1], 1.0f))),
					baseRenderSystem.TransformVec4GLMToVec2SFML(baseRenderSystem.TransformVCSToSCS(glm::vec4((*it).m_Vertices[2], 1.0f))),
					(*it).m_Color)
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
	sf::ConvexShape triangle(3);
	triangle.setPoint(0, surface.m_Vertices[0]);
	triangle.setPoint(1, surface.m_Vertices[1]);
	triangle.setPoint(2, surface.m_Vertices[2]);
	window.draw(triangle);
}
