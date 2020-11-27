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
	std::array<std::array<float, WINDOW_WIDTH>, WINDOW_HEIGHT> m_ZBuffer;
	std::vector<EdgeBucket> m_ActiveEdges;

	ZBufferRenderSystem();
	~ZBufferRenderSystem() = default;

	void Update(entt::registry& registry, BaseRenderSystem& baseRenderSystem, std::vector<SurfaceComponent>& surfacesVCS);

	void Render(entt::registry& registry, BaseRenderSystem& baseRenderSystem, sf::RenderWindow& window);

	void ResetZBuffer();

	class EdgeBucketResult
	{
	public:
		int m_YMin;
		int m_YMax;
		int m_XOfYMin;
		int m_DX;
		int m_DY;
		float m_ZOfYMin;
	};
	EdgeBucketResult GetEdgeBucket(glm::vec3& a, glm::vec3& b);
};

#endif
