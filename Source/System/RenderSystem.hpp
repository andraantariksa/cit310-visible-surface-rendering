#ifndef __CORE_SYSTEM_RENDERSYSTEM_HPP
#define __CORE_SYSTEM_RENDERSYSTEM_HPP

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>

#include "../Component/TransformComponent.hpp"
#include "../Core/Camera.hpp"

class RenderSystem
{
public:
	// Viewer Transform * Screen Transform
	glm::mat4 m_MatTransform;
	Camera m_Camera;

	RenderSystem();
	~RenderSystem() = default;

	// need refactor
	void ResetTransform(entt::registry& registry);
	// default vanishing point
	void ResetMatrix(float vanishing_point_z = -500.0f);

	void Render(entt::registry& registry, sf::RenderWindow& window);
	sf::Vector2f Normalize3DToProjectionSFML(const glm::vec4& v, const TransformComponent& transform);
	glm::vec4 Normalize3DToProjection3D(const glm::vec4& v, const TransformComponent& transform);
	glm::vec4 Normalize3DToProjection(const glm::vec4& v, const TransformComponent& transform);
};

#endif
