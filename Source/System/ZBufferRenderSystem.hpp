#ifndef __SYSTEM_ZBUFFERRENDERSYSTEM_HPP
#define __SYSTEM_ZBUFFERRENDERSYSTEM_HPP

#include <vector>
#include <array>
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "../Misc/EdgeBucket.hpp"
#include "../Macro.hpp"
#include "../Component/SurfaceComponent.hpp"
#include "../Component/Surface2DComponent.hpp"

// Forward declaration
class BaseRenderSystem;

// Read this https://github.com/andraantariksa/cit207-polygon-paint
class ZBufferRenderSystem
{
public:
	sf::Texture m_Texture;
	sf::Sprite m_Sprite;
	std::array<std::array<float, WINDOW_WIDTH>, WINDOW_HEIGHT> m_ZBuffer;
	std::array<sf::Uint8, WINDOW_WIDTH * 4 * WINDOW_HEIGHT> m_TexturePixels;
	//std::array<std::vector<EdgeBucket>, WINDOW_HEIGHT> m_SortedEdgeArray;
	std::vector<EdgeBucket> m_ActiveEdges;

	ZBufferRenderSystem();
	~ZBufferRenderSystem() = default;

	void Update(entt::registry& registry, BaseRenderSystem& baseRenderSystem, std::vector<SurfaceComponent>& surfacesVCS);

	void Render(entt::registry& registry, sf::RenderWindow& window);

	class EdgeBucketResult
	{
	public:
		size_t m_YMin;
		size_t m_YMax;
		size_t m_XOfYMin;
		int m_DX;
		int m_DY;
	};
	EdgeBucketResult GetEdgeBucket(glm::vec3& a, glm::vec3& b);

	void Clear();
	void SetPixel(glm::uvec2 const& position, sf::Color color);
};

#endif
