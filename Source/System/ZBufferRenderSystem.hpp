#ifndef __SYSTEM_ZBUFFERRENDERSYSTEM_HPP
#define __SYSTEM_ZBUFFERRENDERSYSTEM_HPP

#include <vector>
#include <array>
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "../Misc/EdgeBucket.hpp"
#include "../Macro.hpp"
#include "../Component/Triangle3DComponent.hpp"

// Forward declaration
class BaseRenderSystem;

// Read this https://github.com/andraantariksa/cit207-polygon-paint
class ZBufferRenderSystem
{
public:
	std::vector<std::vector<double>> m_ZBuffer;
	std::vector<EdgeBucket> m_ActiveEdges;

	ZBufferRenderSystem();
	~ZBufferRenderSystem() = default;

	void Update(entt::registry& registry, BaseRenderSystem& baseRenderSystem, std::vector<Triangle3DComponent>& surfacesSCS);

	void Render(entt::registry& registry, BaseRenderSystem& baseRenderSystem, sf::RenderWindow& window);

	void ResetZBuffer();

	std::tuple<EdgeBucket, int> GetEdgeBucketAndYMin(glm::dvec3& a, glm::dvec3& b);
};

#endif
