#include "PainterRenderSystem.hpp"

#include <glm/glm.hpp>
#include <tuple>
#include <graphviz/gvc.h>
#include <graphviz/cgraph.h>
#include <graphviz/gvplugin.h>
#include <sstream>
#include <cstdio>

#include "BaseRenderSystem.hpp"
#include "../Component/TransformComponent.hpp"
#include "../Component/Shape3DComponent.hpp"
#include "../Component/SurfaceComponent.hpp"
#include "../Component/Surface2DComponent.hpp"
#include "../Misc/BinaryTreePartitioning.hpp"
#include "../Util/Logger.hpp"
#include "../Misc/EdgeBucket.hpp"

void PainterRenderSystem::Update(entt::registry& registry, BaseRenderSystem& baseRenderSystem, std::vector<SurfaceComponent>& surfacesVCS)
{
	baseRenderSystem.TextureClear();
	m_CachedSurfaces2D.clear();
	m_BinaryPartitioningTree.Clear();

	m_BinaryPartitioningTree.Construct(surfacesVCS);

	std::function<void(std::vector<SurfaceComponent>&)> traverseLambda = [&](std::vector<SurfaceComponent>& surfaces) -> void {
		for (auto& surface : surfaces)
		{
			m_CachedSurfaces2D.push_back(
				SurfaceComponent(
					baseRenderSystem.TransformVCSToSCS(glm::vec4(surface.m_Vertices[0], 1.0f)),
					baseRenderSystem.TransformVCSToSCS(glm::vec4(surface.m_Vertices[1], 1.0f)),
					baseRenderSystem.TransformVCSToSCS(glm::vec4(surface.m_Vertices[2], 1.0f)),
					surface.m_Color)
			);
		}
	};

	m_BinaryPartitioningTree.Traverse(traverseLambda);

	#pragma omp for
	for (auto& surface : m_CachedSurfaces2D)
	{
		auto [yMinOfPolygon, yMaxOfPolygon] = std::minmax_element(
			surface.m_Vertices.begin(),
			surface.m_Vertices.end(),
			[](glm::vec3& coord, glm::vec3& anotherCoord) {
				return coord.y < anotherCoord.y;
			}
		);

		m_ActiveEdges.clear();
		std::vector<std::vector<EdgeBucket2>> sortedEdgeArray(yMaxOfPolygon->y - yMinOfPolygon->y + 1);

		// Traverse on edge
		glm::vec3 prev = *std::prev(surface.m_Vertices.end());
		for (glm::vec3 current : surface.m_Vertices)
		{
			if ((int)prev.y - (int)current.y == 0)
			{
				prev = current;
				continue;
			}

			auto [result, yMin] = GetEdgeBucket(prev, current);
			result.m_YMax -= (int)yMinOfPolygon->y;
			sortedEdgeArray[yMin - (int)yMinOfPolygon->y].push_back(
				std::move(result)
			);

			prev = current;
		}

		for (int y = 0; y < sortedEdgeArray.size(); ++y)
		{
			std::sort(
				m_ActiveEdges.begin(),
				m_ActiveEdges.end(),
				[](const EdgeBucket2& edgeBucket, const EdgeBucket2& otherEdgeBucket) {
					return edgeBucket.m_XOfYMin < otherEdgeBucket.m_XOfYMin;
				}
			);

			#pragma omp for
			for (auto edgeBucket = m_ActiveEdges.begin(); edgeBucket != m_ActiveEdges.end(); ++(++edgeBucket))
			{
				auto nextEdgeBucket = std::next(edgeBucket);

				/*if (nextEdgeBucket == m_ActiveEdges.end())
				{
					break;
				}*/

				for (int x = edgeBucket->m_XOfYMin; x <= nextEdgeBucket->m_XOfYMin; ++x)
				{
					const auto position = glm::ivec2(x, y + (int)yMinOfPolygon->y);
					baseRenderSystem.TextureSetPixel(position, surface.m_Color);
				}
			}

			m_ActiveEdges.erase(
				std::remove_if(
					m_ActiveEdges.begin(),
					m_ActiveEdges.end(),
					[&](EdgeBucket2& edgeBucket)
					{
						edgeBucket.NextX();
						return !edgeBucket.IsAlive(y);
					}
				),
				m_ActiveEdges.end()
			);

			m_ActiveEdges.insert(m_ActiveEdges.end(), sortedEdgeArray[y].begin(), sortedEdgeArray[y].end());
		}
	}

	baseRenderSystem.m_Texture.update(baseRenderSystem.m_TexturePixels.data());
}

void PainterRenderSystem::Render(entt::registry& registry, BaseRenderSystem& baseRenderSystem, sf::RenderWindow& window)
{
	window.draw(baseRenderSystem.m_Sprite);
	/*sf::VertexArray vertArray(sf::Triangles);
	for (auto& surface : m_CachedSurfaces2D)
	{
		vertArray.append(
			sf::Vertex(
				sf::Vector2f(surface.m_Vertices[0].x, surface.m_Vertices[0].y), surface.m_Color
			));
		vertArray.append(
			sf::Vertex(
				sf::Vector2f(surface.m_Vertices[1].x, surface.m_Vertices[1].y), surface.m_Color
			));
		vertArray.append(
			sf::Vertex(
				sf::Vector2f(surface.m_Vertices[2].x, surface.m_Vertices[2].y), surface.m_Color
			));
	}
	window.draw(vertArray);*/
}

void PainterRenderSystem::PrintBinaryPartitioningTree()
{
	GVC_t* gvc = gvContext();
	//gvAddLibrary(gvc, &gvplugin_dot_layout_LTX_library);
	assert(gvc);
	Agraph_t* graph = agopen((char*)"BSP Tree", Agdirected, nullptr);
	assert(graph);

	INFO << "Print BSP\n";
	m_BinaryPartitioningTree.GetCGraphTree(graph);

	assert(gvLayout(gvc, graph, "dot"));
	// Save as PNG
	assert(gvRender(gvc, graph, "png", fopen("result.png", "w")));
	// Free the memory
	gvFreeLayout(gvc, graph);
	agclose(graph);

	gvFreeContext(gvc);
}

std::tuple<EdgeBucket2, int> PainterRenderSystem::GetEdgeBucket(glm::vec3& a, glm::vec3& b)
{
	EdgeBucket2 edgeBucket;
	int yMin;
	if (a.y < b.y)
	{
		yMin = (int)a.y;
		edgeBucket.m_XOfYMin = (int)a.x;
	}
	else
	{
		yMin = (int)b.y;
		edgeBucket.m_XOfYMin = (int)b.x;
	}
	edgeBucket.m_YMax = std::max((int)a.y, (int)b.y);
	edgeBucket.m_DX = (int)a.x - (int)b.x;
	edgeBucket.m_DY = (int)a.y - (int)b.y;
	if (edgeBucket.m_DY < 0)
	{
		edgeBucket.m_DY *= -1;
		edgeBucket.m_DX *= -1;
	}
	return std::move(std::tuple<EdgeBucket2, int>({edgeBucket, yMin}));
}
