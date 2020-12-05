#ifndef __SYSTEM_BASERENDERSYSTEM_HPP
#define __SYSTEM_BASERENDERSYSTEM_HPP

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <optional>

#include "../Component/TransformComponent.hpp"
#include "../Component/Surface3DComponent.hpp"
#include "TransformSystem.hpp"
#include "PainterRenderSystem.hpp"
#include "ZBufferRenderSystem.hpp"

class BaseRenderSystem
{
public:
	// Careful. Use a lot of stack memory
	sf::Texture m_Texture;
	sf::Sprite m_Sprite;
	std::array<sf::Uint8, WINDOW_WIDTH * 4 * WINDOW_HEIGHT> m_TexturePixels;

	TransformSystem m_SystemTransform;
	ZBufferRenderSystem m_SystemZBufferRender;
	PainterRenderSystem m_SystemPainterRender;

	// Stored in the class to avoid reallocation
	std::vector<Shape3DComponent> m_Shapes3DVCS;

	enum class RenderMethod: int
	{
		None,
		Painter,
		ZBuffer
	};

	RenderMethod m_RenderMethod;
	
	// WCS to VCS Matrix * VCS to SCS Matrix
	glm::dmat4 m_MatTransform;
	glm::dmat4 m_MatWCSToVCS;
	glm::dmat4 m_MatVCSToSCS;

	BaseRenderSystem(RenderMethod renderMethod=RenderMethod::Painter, double vanishingPointZ=500.0);
	~BaseRenderSystem() = default;

	void Update(entt::registry& registry);
	void Render(entt::registry& registry, sf::RenderWindow& window);
	void ChangeRenderMethod(entt::registry& registry, RenderMethod renderMethod);

	// Default vanishing point
	void ResetMatrix(double vanishing_point_z = -500.0);

	bool IsSurfaceIsBackFaceCulled(const std::array<glm::dvec3, 3>& triangle);

	sf::Vector2f TransformVec4GLMToVec2SFML(const glm::dvec4& v);
	glm::dvec4 TransformVCSToSCS(const glm::dvec4& v);
	glm::dvec4 TransformWCSToVCS(const glm::dvec4& v);
	glm::dvec4 TransformWCSToSCS(const glm::dvec4& v);
	glm::dvec4 TransformOCSToWCS(const glm::dvec4& v, const TransformComponent& transform);

	void TextureClear();
	void TextureSetPixel(const glm::ivec2& position, const sf::Color& color);

	void Print();
};

#endif
