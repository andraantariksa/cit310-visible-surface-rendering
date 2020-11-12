#ifndef __SYSTEM_PAINTERRENDERSYSTEM_HPP
#define __SYSTEM_PAINTERRENDERSYSTEM_HPP

#include <vector>
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

#include "../Component/SurfaceComponent.hpp"
#include "../Component/Surface2DComponent.hpp"

// Forward declaration
class BaseRenderSystem;

class PainterRenderSystem
{
public:
	PainterRenderSystem() = default;
	~PainterRenderSystem() = default;

	std::vector<Surface2DComponent> m_CachedSurfaces2D;

	void Update(entt::registry& registry, BaseRenderSystem& baseRenderSystem, std::vector<SurfaceComponent>& surfacesVCS);

	void Render(entt::registry& registry, sf::RenderWindow& window);

	void DrawSurface(sf::RenderWindow& window, const Surface2DComponent& surface);
};

#endif
