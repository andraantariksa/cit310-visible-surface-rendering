#ifndef __CORE_SYSTEM_RENDERSYSTEM_HPP
#define __CORE_SYSTEM_RENDERSYSTEM_HPP

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>

#include "../Component/TransformComponent.hpp"

class RenderSystem
{
public:
	// Viewer Transform * Screen Transform
	glm::mat4 m_MatTransform;

	RenderSystem();
	~RenderSystem() = default;
	void Render(entt::registry& registry, sf::RenderWindow& window);
	inline sf::Vector2f Normalize3DToProjection(const glm::vec4& v, const TransformComponent& transform);
};

#endif
