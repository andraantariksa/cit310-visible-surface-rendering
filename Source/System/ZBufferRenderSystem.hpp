#ifndef __SYSTEM_ZBUFFERRENDERSYSTEM_HPP
#define __SYSTEM_ZBUFFERRENDERSYSTEM_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "../Component/Surface2DComponent.hpp"
#include "../Macro.hpp"

// Read this https://github.com/andraantariksa/cit207-polygon-paint
// TODO: Move out somewhere else
class SortedEdge
{
};

class ZBufferRenderSystem
{
public:
	sf::Texture m_Texture;
	sf::Sprite m_Sprite;
	std::array<std::array<float, WINDOW_WIDTH>, WINDOW_HEIGHT> m_ZBuffer;
	std::array<sf::Uint8, WINDOW_WIDTH * 4 * WINDOW_HEIGHT> m_TexturePixels;
	std::array<std::vector<SortedEdge>, WINDOW_HEIGHT> m_SortedEdgeArray;
	std::vector<SortedEdge> m_ActiveEdgeArray;

	ZBufferRenderSystem();
	~ZBufferRenderSystem() = default;

	void Update(entt::registry& registry);

	void Render(entt::registry& registry, sf::RenderWindow& window);

	void Clear();
	void SetPixel(sf::Vertex point);
	void DrawSurface(const Surface2DComponent& surface);
};

#endif
