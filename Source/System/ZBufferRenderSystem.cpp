#include "ZBufferRenderSystem.hpp"

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <cstring>
#include <cmath>

#include "../Macro.hpp"

ZBufferRenderSystem::ZBufferRenderSystem():
	m_Sprite(m_Texture)
{
	Clear();
}

void ZBufferRenderSystem::Update(entt::registry& registry)
{
	Clear();
}

void ZBufferRenderSystem::Render(entt::registry& registry, sf::RenderWindow& window)
{
	window.draw(m_Sprite);
}

void ZBufferRenderSystem::Clear()
{
	// Reset ZBuffer
	for (auto& it = m_ZBuffer.begin(); it != m_ZBuffer.end(); ++it)
	{
		for (auto& it2 = (*it).begin(); it2 != (*it).end(); ++it2)
		{
			*it2 = -INFINITY;
		}
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

void ZBufferRenderSystem::SetPixel(sf::Vertex point)
{
	const size_t currentPixelPos = (point.position.x + (point.position.y * WINDOW_WIDTH)) * 4;
	m_TexturePixels[currentPixelPos] = point.color.r;
	m_TexturePixels[currentPixelPos + 1L] = point.color.g;
	m_TexturePixels[currentPixelPos + 2L] = point.color.b;
	m_TexturePixels[currentPixelPos + 3L] = point.color.a;
}

void ZBufferRenderSystem::DrawSurface(const Surface2DComponent& surface)
{
}
