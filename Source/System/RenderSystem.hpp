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
	Camera m_Camera;

	RenderSystem();
	~RenderSystem() = default;

	void Render(entt::registry& registry, sf::RenderWindow& window);
};

#endif
