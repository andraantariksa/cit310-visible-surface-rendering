#include "ZBufferRenderSystem.hpp"

#include <array>
#include <vector>
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <tuple>
#include <glm/gtx/string_cast.hpp>

#include "BaseRenderSystem.hpp"
#include "../Misc/EdgeBucket.hpp"
#include "../Component/Shape3DComponent.hpp"
#include "../Component/Triangle3DComponent.hpp"
#include "../Macro.hpp"
#include "../Util/Logger.hpp"

ZBufferRenderSystem::ZBufferRenderSystem()
{
	ResetZBuffer();
}

void ZBufferRenderSystem::Update(entt::registry& registry, BaseRenderSystem& baseRenderSystem, std::vector<Triangle3DComponent>& surfacesSCS)
{
	baseRenderSystem.TextureClear();
	ResetZBuffer();

	#pragma omp for
	for (auto& surface : surfacesSCS)
	{
		glm::dvec3 normal = glm::cross(
			surface.m_Vertices[1] - surface.m_Vertices[0],
			surface.m_Vertices[2] - surface.m_Vertices[1]
		);
		double dZX = -normal.x / normal.z;

		auto [yMinOfPolygon, yMaxOfPolygon] = std::minmax_element(
			surface.m_Vertices.begin(),
			surface.m_Vertices.end(),
			[](glm::dvec3& coord, glm::dvec3& anotherCoord) {
				return coord.y < anotherCoord.y;
			}
		);

		m_ActiveEdges.clear();
		std::vector<std::vector<EdgeBucket>> sortedEdgeArray(yMaxOfPolygon->y - yMinOfPolygon->y + 1);

		// Traverse on edge
		glm::dvec3 prev = *std::prev(surface.m_Vertices.end());
		for (glm::dvec3 current : surface.m_Vertices)
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
			m_ActiveEdges.erase(
				std::remove_if(
					m_ActiveEdges.begin(),
					m_ActiveEdges.end(),
					[&](EdgeBucket& edgeBucket)
					{
						edgeBucket.NextX(normal.x, normal.y, normal.z);
						return !edgeBucket.IsAlive(y);
					}
				),
				m_ActiveEdges.end());

			m_ActiveEdges.insert(m_ActiveEdges.end(), sortedEdgeArray[y].begin(), sortedEdgeArray[y].end());

			std::sort(
				m_ActiveEdges.begin(),
				m_ActiveEdges.end(),
				[](const EdgeBucket& edgeBucket, const EdgeBucket& otherEdgeBucket) {
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

				double z = edgeBucket->m_ZOfYMin;
				for (int x = edgeBucket->m_XOfYMin; x <= nextEdgeBucket->m_XOfYMin; ++x)
				{
					const auto position = glm::ivec2(x, y + (int)yMinOfPolygon->y);
					if (!(position.x >= WINDOW_WIDTH || position.y >= WINDOW_HEIGHT ||
						position.x < 0 || position.y < 0))
					{
						if (z > m_ZBuffer[position.y][position.x])
						{
							baseRenderSystem.TextureSetPixel(position, surface.m_Color);
							m_ZBuffer[position.y][position.x] = z;
						}
					}
					z += dZX;
				}
			}
		}
	}

	baseRenderSystem.m_Texture.update(baseRenderSystem.m_TexturePixels.data());
}

void ZBufferRenderSystem::Render(entt::registry& registry, BaseRenderSystem& baseRenderSystem, sf::RenderWindow& window)
{
	window.draw(baseRenderSystem.m_Sprite);
}

void ZBufferRenderSystem::ResetZBuffer()
{
	// Reset ZBuffer
	for (auto& it : m_ZBuffer)
	{
		for (auto& it2: it)
		{
			it2 = -INFINITY;
		}
	}
}

std::tuple<EdgeBucket, int> ZBufferRenderSystem::GetEdgeBucket(glm::dvec3& a, glm::dvec3& b)
{
	EdgeBucket edgeBucket;
	int yMin;
	if (a.y < b.y)
	{
		yMin = (int)a.y;
		edgeBucket.m_XOfYMin = (int)a.x;
		edgeBucket.m_ZOfYMin = a.z;
	}
	else
	{
		yMin = (int)b.y;
		edgeBucket.m_XOfYMin = (int)b.x;
		edgeBucket.m_ZOfYMin = b.z;
	}
	edgeBucket.m_YMax = (int)std::max(a.y, b.y);
	edgeBucket.m_DX = (int)a.x - (int)b.x;
	edgeBucket.m_DY = (int)a.y - (int)b.y;
	if (edgeBucket.m_DY < 0)
	{
		edgeBucket.m_DY *= -1;
		edgeBucket.m_DX *= -1;
	}
	return std::move(std::tuple<EdgeBucket, int>({ edgeBucket, yMin }));
}
