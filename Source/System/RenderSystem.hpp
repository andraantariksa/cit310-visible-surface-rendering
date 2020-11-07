#ifndef __CORE_SYSTEM_RENDERSYSTEM_HPP
#define __CORE_SYSTEM_RENDERSYSTEM_HPP

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <optional>

#include "../Component/TransformComponent.hpp"
#include "../Core/Camera.hpp"

class RenderSystem
{
public:
	// Viewer Transform * Screen Transform
	glm::mat4 m_MatWCSToWCS2;
	glm::mat4 m_MatTransform;
	glm::mat4 m_MatWCSToVCS;
	glm::mat4 m_MatVCSToSCS;
	Camera m_Camera;

	RenderSystem();
	~RenderSystem() = default;

	// need refactor
	void ResetTransform(entt::registry& registry);
	// default vanishing point
	void ResetMatrix(float vanishing_point_z = -500.0f);

	void Render(entt::registry& registry, sf::RenderWindow& window);
	sf::Vector2f TransformToSCSVec2SFML(const glm::vec4& v, const TransformComponent& transform);
	glm::vec2 RenderSystem::TransformToSCSVec2GLM(const glm::vec4& v, const TransformComponent& transform);
	glm::vec4 TransformToVCSVec4(const glm::vec4& v, const TransformComponent& transform);
	glm::vec4 TransformToWCSVec4(const glm::vec4& v, const TransformComponent& transform);
	glm::vec4 TransformToSCS(const glm::vec4& v, const TransformComponent& transform);
	glm::vec4 TransformToVCS(const glm::vec4& v, const TransformComponent& transform);
	glm::vec4 TransformToWCS(const glm::vec4& v, const TransformComponent& transform);

	std::optional<glm::vec3> GetSphereSurfaceNormal(entt::registry& registry, glm::vec2& screen_coord);
};

#endif
