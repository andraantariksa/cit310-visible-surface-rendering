#ifndef __CORE_SYSTEM_RENDERSYSTEM_HPP
#define __CORE_SYSTEM_RENDERSYSTEM_HPP

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

class RenderSystem
{
public:
	RenderSystem() = default;
	~RenderSystem() = default;
	void Render(sf::RenderWindow* window, entt::basic_registry<entt::entity>& registry);
};

#endif
