#include "App.hpp"

#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <glm/glm.hpp>

#include "../Macro.hpp"
#include "../ExampleShape.hpp"
#include "../Component/TransformComponent.hpp"
#include "../Component/Shape3DComponent.hpp"
#include "../Util/Logger.hpp"

App::App():
	m_Window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), APP_NAME, sf::Style::Titlebar | sf::Style::Close),
    m_ClearColor(96, 96, 96)
{
	m_Window.setFramerateLimit(15);
    ImGui::SFML::Init(m_Window);
    ImGui::GetIO().IniFilename = nullptr;

    // Initialize default 2 pyramid
    const std::vector<SurfaceComponent> pyramidSurfaces = Shape3DExample();

    const entt::entity pyramid1 = m_Registry.create();
    m_Registry.emplace<Shape3DComponent>(pyramid1, pyramidSurfaces);
    m_Registry.emplace<TransformComponent>(pyramid1);
    m_Entities[0] = pyramid1;

    const entt::entity pyramid2 = m_Registry.create();
    m_Registry.emplace<Shape3DComponent>(pyramid2, pyramidSurfaces);
    m_Registry.emplace<TransformComponent>(pyramid2);
    m_Entities[2] = pyramid2;
}

App::~App()
{
    ImGui::SFML::Shutdown();
}

void App::Run()
{
	while (m_Window.isOpen())
	{
        sf::Event event;
        bool rightClick = false;
        while (m_Window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
            {
                m_Window.close();
            }
        }

        UpdateInterface();
        Update();

        m_Window.clear(m_ClearColor);
        Render();

        m_DeltaClock.restart();
	}
}

void App::UpdateInterface()
{
    ImGui::SFML::Update(m_Window, m_DeltaClock.getElapsedTime());
}

void App::Update()
{
}

void App::Render()
{
    m_SystemRender.Render(m_Registry, m_Window);
    ImGui::SFML::Render(m_Window);
    m_Window.display();
}
