#ifndef __CORE_APP_HPP
#define __CORE_APP_HPP

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "../System/RenderSystem.hpp"

class App
{
private:
	sf::RenderWindow window;
	sf::Clock deltaClock;
	entt::registry registry;

	// System
	RenderSystem system_render;
public:
	App();
	~App();

	void Run();
	void UpdateInterface();
	void Render();
};

#endif
