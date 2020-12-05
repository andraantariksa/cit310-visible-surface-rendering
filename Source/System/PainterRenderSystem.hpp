#ifndef __SYSTEM_PAINTERRENDERSYSTEM_HPP
#define __SYSTEM_PAINTERRENDERSYSTEM_HPP

#include <vector>
#include <tuple>
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

#include "../Macro.hpp"
#include "../Component/Triangle2DComponent.hpp"
#include "../Component/Triangle3DComponent.hpp"
#include "../Component/Shape3DComponent.hpp"
#include "../Misc/EdgeBucket.hpp"
#include "../Misc/BinaryTreePartitioning.hpp"

// Forward declaration
class BaseRenderSystem;

class PainterRenderSystem
{
public:
	BinaryTreePartitioning m_BinaryPartitioningTree;

	std::vector<Triangle2DComponent> m_CachedSurfaces3D;

	PainterRenderSystem() = default;
	~PainterRenderSystem() = default;

	void Update(entt::registry& registry, BaseRenderSystem& baseRenderSystem, std::vector<Triangle3DComponent>& surfacesVCS);

	void Render(entt::registry& registry, BaseRenderSystem& baseRenderSystem, sf::RenderWindow& window);

	void PrintBinaryPartitioningTree();
};

#endif
