#ifndef __SYSTEM_PAINTERRENDERSYSTEM_HPP
#define __SYSTEM_PAINTERRENDERSYSTEM_HPP

#include <vector>
#include <tuple>
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <graphviz/gvc.h>
#include <graphviz/cgraph.h>
#include <graphviz/gvplugin.h>

#include "../Macro.hpp"
#include "../Component/SurfaceComponent.hpp"
#include "../Component/Surface2DComponent.hpp"
#include "../Misc/EdgeBucket.hpp"
#include "../Misc/BinaryTreePartitioning.hpp"

//extern gvplugin_library_t gvplugin_dot_layout_LTX_library;

// Forward declaration
class BaseRenderSystem;

class PainterRenderSystem
{
public:
	std::vector<EdgeBucket2> m_ActiveEdges;

	BinaryTreePartitioning m_BinaryPartitioningTree;

	std::vector<SurfaceComponent> m_CachedSurfaces2D;

	PainterRenderSystem() = default;
	~PainterRenderSystem() = default;

	void Update(entt::registry& registry, BaseRenderSystem& baseRenderSystem, std::vector<SurfaceComponent>& surfacesVCS);

	void Render(entt::registry& registry, BaseRenderSystem& baseRenderSystem, sf::RenderWindow& window);

	void PrintBinaryPartitioningTree();

	std::tuple<EdgeBucket2, int> GetEdgeBucket(glm::vec3& a, glm::vec3& b);
};

#endif
