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

ZBufferRenderSystem::ZBufferRenderSystem() :
	m_Sprite(m_Texture)
{
	Clear();
}

void ZBufferRenderSystem::Update(entt::registry& registry, BaseRenderSystem& baseRenderSystem, std::vector<SurfaceComponent>& surfacesVCS)
{
	Clear();

	for (auto& surface : surfacesVCS)
	{
		// Traverse on edge
		glm::vec3& prev = *--surface.m_Vertices.end();
		for (glm::vec3& current : surface.m_Vertices)
		{
			if (prev.y == current.y)
			{
				continue;
			}
			ZBufferRenderSystem::EdgeBucketResult result = GetEdgeBucket(prev, current);
			EdgeBucket edgeBucket{
				.m_YMax = result.m_YMax,
				.m_XOfYMin = result.m_XOfYMin,
				.m_DX = result.m_DX,
				.m_DY = result.m_DY,
				.m_Carry = 0
			};
			INFO << result.m_YMin << " " << edgeBucket.m_XOfYMin << '\n';
			m_SortedEdgeArray[result.m_YMin].push_back(edgeBucket);
		}
	}

	// Care underflow
	for (size_t y = WINDOW_HEIGHT; y >= 0; --y)
	{
		std::sort(
			m_ActiveEdges.begin(),
			m_ActiveEdges.end(),
			[](EdgeBucket& edgeBucket, EdgeBucket& otherEdgeBucket) {
				return edgeBucket.m_XOfYMin <= otherEdgeBucket.m_XOfYMin;
			}
		);

		for (auto& edgeBucket = m_ActiveEdges.begin(); edgeBucket != m_ActiveEdges.end(); ++(++edgeBucket))
		{
			auto& nextEdgeBucket = std::next(edgeBucket);
			for (size_t x = edgeBucket->m_XOfYMin; x <= nextEdgeBucket->m_XOfYMin; ++x)
			{
				SetPixel(glm::uvec2(x, y), sf::Color::Red);
			}
		}
		
		m_ActiveEdges.insert(m_ActiveEdges.end(), m_SortedEdgeArray[y].begin(), m_SortedEdgeArray[y].end());
	}
}

void ZBufferRenderSystem::Render(entt::registry& registry, sf::RenderWindow& window)
{
	window.draw(m_Sprite);
}

ZBufferRenderSystem::EdgeBucketResult ZBufferRenderSystem::GetEdgeBucket(glm::vec3& a, glm::vec3& b)
{
	INFO << glm::to_string(a) << '\n';
	INFO << glm::to_string(b) << '\n';
	EdgeBucketResult result;
	if (a.y < b.y)
	{
		result.m_YMin = (size_t)a.y;
		result.m_XOfYMin = (size_t)a.x;
	}
	else
	{
		result.m_YMin = (size_t)b.y;
		result.m_XOfYMin = (size_t)b.x;
	}
	INFO << "x of y min " << result.m_XOfYMin << " " << result.m_YMin << '\n';
	result.m_YMax = (size_t)std::max(a.y, b.y);
	result.m_DX = (size_t)(a.x - b.x);
	result.m_DY = (size_t)(a.y - b.y);
	if (result.m_DY < 0)
	{
		result.m_DY *= -1;
		result.m_DX *= -1;
	}
	INFO << "x of y min " << result.m_XOfYMin << " " << result.m_YMin << '\n';
	return std::move(result);
}

void ZBufferRenderSystem::Clear()
{
	// Reset ZBuffer
	for (auto& it = m_ZBuffer.begin(); it != m_ZBuffer.end(); ++it)
	{
		for (auto& it2 = it->begin(); it2 != it->end(); ++it2)
		{
			*it2 = -INFINITY;
		}
	}

	for (auto& edgeBuckets: m_SortedEdgeArray)
	{
		edgeBuckets.clear();
	}

	sf::Color clearColor(CLEAR_COLOR);
	for (size_t currentPixelPos = 0; currentPixelPos < WINDOW_WIDTH * WINDOW_HEIGHT * 4; currentPixelPos += 4)
	{
		m_TexturePixels[currentPixelPos] = clearColor.r;
		m_TexturePixels[currentPixelPos + 1] = clearColor.g;
		m_TexturePixels[currentPixelPos + 2] = clearColor.b;
		m_TexturePixels[currentPixelPos + 3] = clearColor.a;
	}

	m_Texture.update(m_TexturePixels.data());
}

void ZBufferRenderSystem::SetPixel(glm::uvec2& position, sf::Color color)
{
	const size_t currentPixelPos = ((size_t)position.x + ((size_t)position.y * WINDOW_WIDTH)) * 4L;
	m_TexturePixels[currentPixelPos] = color.r;
	m_TexturePixels[currentPixelPos + 1L] = color.g;
	m_TexturePixels[currentPixelPos + 2L] = color.b;
	m_TexturePixels[currentPixelPos + 3L] = color.a;
}
