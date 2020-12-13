#ifndef __CORE_APP_HPP
#define __CORE_APP_HPP

#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <memory>

#include "../System/BaseRenderSystem.hpp"
#include "../System/TransformSystem.hpp"

class App
{
private:
	sf::RenderWindow m_Window;
	sf::Clock m_DeltaClock;
	entt::registry m_Registry;
	sf::Color m_ClearColor;
	std::array<std::unique_ptr<entt::entity>, 9> m_Entities;
	int m_SelectedEntityIdx;

	int m_GUIRenderMethod;
	enum class TransformationMethod : int
	{
		Rotation,
		Translation
	};
	int m_GUITransformationMethod;
	bool m_GUIWindowTips;
	double m_GUITransformStep;
	double m_GUIZC;
	bool m_GUIShowMessageBox;
	std::string m_GUIMessageText;

	// System
	BaseRenderSystem m_SystemRender;
	TransformSystem m_SystemTransform;
public:
	App();
	~App();

	void Run();
	void UpdateInterface();
	void Update();
	void ShowMessageBox(std::string& messageText);
	void Render();
};

#endif
