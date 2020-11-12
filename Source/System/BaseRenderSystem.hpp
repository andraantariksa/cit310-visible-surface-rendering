#ifndef __SYSTEM_BASERENDERSYSTEM_HPP
#define __SYSTEM_BASERENDERSYSTEM_HPP

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <optional>

#include "../Component/TransformComponent.hpp"
#include "../Component/SurfaceComponent.hpp"
#include "../Core/Camera.hpp"
#include "TransformSystem.hpp"
#include "PainterRenderSystem.hpp"
#include "ZBufferRenderSystem.hpp"

class BaseRenderSystem
{
public:
	TransformSystem m_SystemTransform;
	ZBufferRenderSystem m_SystemZBufferRender;
	PainterRenderSystem m_SystemPainterRender;

	// Stored in the class to avoid reallocation
	std::vector<SurfaceComponent> m_SurfacesVCS;

	enum class RenderMethod
	{
		None,
		Painter,
		ZBuffer
	};

	RenderMethod m_RenderMode;
	
	// WCS to VCS Matrix * VCS to SCS Matrix
	glm::mat4 m_MatTransform;
	glm::mat4 m_MatWCSToVCS;
	glm::mat4 m_MatVCSToSCS;

	Camera m_Camera;

	BaseRenderSystem(float vanishingPointZ=500.0f);
	~BaseRenderSystem() = default;

	void Update(entt::registry& registry);
	void Render(entt::registry& registry, sf::RenderWindow& window);

	// Default vanishing point
	void ResetMatrix(float vanishing_point_z = -500.0f);

	bool IsSurfaceIsBackFaceCulled(const SurfaceComponent& surface);

	sf::Vector2f TransformVec4GLMToVec2SFML(const glm::vec4& v);
	glm::vec4 TransformVCSToSCS(const glm::vec4& v);
	glm::vec4 TransformWCSToVCS(const glm::vec4& v);
	glm::vec4 TransformOCSToWCS(const glm::vec4& v, const TransformComponent& transform);
};

#endif
