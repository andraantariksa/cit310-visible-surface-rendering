#include "ZBufferRenderSystem.hpp"

#include <array>
#include <vector>
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <glm/gtx/string_cast.hpp>

#include "BaseRenderSystem.hpp"
#include "../Misc/EdgeBucket.hpp"
#include "../Component/SurfaceComponent.hpp"
#include "../Macro.hpp"
#include "../Util/Logger.hpp"

ZBufferRenderSystem::ZBufferRenderSystem()
{
	ResetZBuffer();
}

void ZBufferRenderSystem::Update(entt::registry& registry, BaseRenderSystem& baseRenderSystem, std::vector<SurfaceComponent>& surfacesVCS)
{
	baseRenderSystem.TextureClear();
	ResetZBuffer();

	#pragma omp for
	for (auto& surface : surfacesVCS)
	{
		// Change it into SCS and preserve the Z value, which will used by the Z buffer
		surface = SurfaceComponent(
			baseRenderSystem.TransformVCSToSCS(glm::vec4(surface.m_Vertices[0], 1.0f)),
			baseRenderSystem.TransformVCSToSCS(glm::vec4(surface.m_Vertices[1], 1.0f)),
			baseRenderSystem.TransformVCSToSCS(glm::vec4(surface.m_Vertices[2], 1.0f)),
			surface.m_Color);

		glm::vec3 normal = glm::cross(
			surface.m_Vertices[1] - surface.m_Vertices[0],
			surface.m_Vertices[2] - surface.m_Vertices[1]
		);
		float dZX = -normal.x / normal.z;

		auto [yMinOfPolygon, yMaxOfPolygon] = std::minmax_element(
			surface.m_Vertices.begin(),
			surface.m_Vertices.end(),
			[](glm::vec3& coord, glm::vec3& anotherCoord) {
				return coord.y < anotherCoord.y;
			}
		);

		m_ActiveEdges.clear();
		std::vector<std::vector<EdgeBucket>> sortedEdgeArray(yMaxOfPolygon->y - yMinOfPolygon->y + 1);

		// Traverse on edge
		glm::vec3 prev = *std::prev(surface.m_Vertices.end());
		for (glm::vec3 current : surface.m_Vertices)
		{
			if ((int)prev.y - (int)current.y == 0)
			{
				prev = current;
				continue;
			}

			ZBufferRenderSystem::EdgeBucketResult result = GetEdgeBucket(prev, current);
			sortedEdgeArray[result.m_YMin - (int)yMinOfPolygon->y].push_back(
				EdgeBucket{
					.m_YMax = result.m_YMax - (int)yMinOfPolygon->y,
					.m_XOfYMin = result.m_XOfYMin,
					.m_DX = result.m_DX,
					.m_DY = result.m_DY,
					.m_Carry = 0,
					.m_ZOfYMin = result.m_ZOfYMin
				}
			);

			prev = current;
		}

		for (int y = 0; y < sortedEdgeArray.size(); ++y)
		{
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
				
				float z = edgeBucket->m_ZOfYMin;
				for (int x = edgeBucket->m_XOfYMin; x <= nextEdgeBucket->m_XOfYMin; ++x)
				{
					const auto position = glm::ivec2(x, y + (int)yMinOfPolygon->y);
					if (z > m_ZBuffer[position.y][position.x])
					{
						baseRenderSystem.TextureSetPixel(position, surface.m_Color);
						m_ZBuffer[position.y][position.x] = z;
					}
					z += dZX;
				}
			}

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
				m_ActiveEdges.end()
			);

			m_ActiveEdges.insert(m_ActiveEdges.end(), sortedEdgeArray[y].begin(), sortedEdgeArray[y].end());
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

ZBufferRenderSystem::EdgeBucketResult ZBufferRenderSystem::GetEdgeBucket(glm::vec3& a, glm::vec3& b)
{
	EdgeBucketResult result;
	if (a.y < b.y)
	{
		result.m_YMin = (int)a.y;
		result.m_XOfYMin = (int)a.x;
		result.m_ZOfYMin = a.z;
	}
	else
	{
		result.m_YMin = (int)b.y;
		result.m_XOfYMin = (int)b.x;
		result.m_ZOfYMin = b.z;
	}
	result.m_YMax = std::max((int)a.y, (int)b.y);
	result.m_DX = (int)a.x - (int)b.x;
	result.m_DY = (int)a.y - (int)b.y;
	if (result.m_DY < 0)
	{
		result.m_DY *= -1;
		result.m_DX *= -1;
	}
	return std::move(result);
}
