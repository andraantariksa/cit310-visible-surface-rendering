#ifndef __CORE_APP_HPP
#define __CORE_APP_HPP

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <optional>

#include "../System/BaseRenderSystem.hpp"

class App
{
private:
	sf::RenderWindow m_Window;
	sf::Clock m_DeltaClock;
	entt::registry m_Registry;
	sf::Color m_ClearColor;
	std::array<std::optional<entt::entity>, 9> m_Entities;
	int m_SelectedEntityIdx;

	// System
	BaseRenderSystem m_SystemRender;
public:
	App();
	~App();

	void Run();
	void UpdateInterface();
	void Update();
	void Render();
};

#endif
