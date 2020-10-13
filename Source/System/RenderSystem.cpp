#include "RenderSystem.hpp"

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <iostream>

#include "../Component/PositionComponent.hpp"
#include "../Component/SphereComponent.hpp"

void RenderSystem::Render(sf::RenderWindow* window, entt::basic_registry<entt::entity>& registry)
{
	registry.view<SphereComponent, PositionComponent>().each([&](auto entity, SphereComponent sphere, PositionComponent pos)
	{
			std::cout << "Hello!\n";
	});
}
