#include "RenderSystem.hpp"

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <iostream>
#include <glm/glm.hpp>

#include "../Component/PositionComponent.hpp"
#include "../Component/SphereComponent.hpp"

void RenderSystem::Render(entt::registry& registry, sf::RenderWindow& window)
{
	registry.view<SphereComponent, PositionComponent>().each([&](auto entity, SphereComponent sphere, PositionComponent pos)
	{
			std::cout << "Hello!\n";
	});
}
