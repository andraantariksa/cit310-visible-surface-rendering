#ifndef __CORE_SYSTEM_RENDERSYSTEM_HPP
#define __CORE_SYSTEM_RENDERSYSTEM_HPP

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

class RenderSystem
{
public:
	RenderSystem() = default;
	~RenderSystem() = default;
	void Render(entt::registry& registry, sf::RenderWindow& window);
};

#endif
