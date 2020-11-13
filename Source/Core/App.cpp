#include "App.hpp"

#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <memory>
#include <glm/glm.hpp>

#include "../Macro.hpp"
#include "../ExampleShape.hpp"
#include "../Component/TransformComponent.hpp"
#include "../Component/Shape3DComponent.hpp"
#include "../System/BaseRenderSystem.hpp"
#include "../Util/Logger.hpp"

App::App():
	m_Window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), APP_NAME, sf::Style::Titlebar | sf::Style::Close),
    m_ClearColor(CLEAR_COLOR),
    // -1 means not selecting anything
    m_SelectedEntityIdx(-1),
    m_GUIRenderMethod((int)BaseRenderSystem::RenderMethod::Painter),
    m_SystemRender((BaseRenderSystem::RenderMethod)m_GUIRenderMethod)
{
	m_Window.setFramerateLimit(15);
    ImGui::SFML::Init(m_Window);
    ImGui::GetIO().IniFilename = nullptr;

    // Initialize default 2 pyramid
    m_Entities[0] = std::make_unique<entt::entity>(m_Registry.create());
    m_Registry.emplace<Shape3DComponent>(*m_Entities[0], Shape3DExample());
    m_Registry.emplace<TransformComponent>(*m_Entities[0]);

    m_SystemTransform.Rotate(m_Registry, *m_Entities[0], glm::vec3(1.0f, 0.0f, 0.0f), 30.0f);
    m_SystemTransform.Translate(m_Registry, *m_Entities[0], glm::vec3(-50.0f, 0.0f, 0.0f));

    m_Entities[1] = std::make_unique<entt::entity>(m_Registry.create());
    m_Registry.emplace<Shape3DComponent>(*m_Entities[1], Shape3DExample());
    m_Registry.emplace<TransformComponent>(*m_Entities[1]);

    m_SystemTransform.Rotate(m_Registry, *m_Entities[1], glm::vec3(1.0f, 0.0f, 0.0f), 30.0f);
    m_SystemTransform.Rotate(m_Registry, *m_Entities[1], glm::vec3(0.0f, 1.0f, 0.0f), 30.0f);
    m_SystemTransform.Translate(m_Registry, *m_Entities[1], glm::vec3(50.0f, 0.0f, 0.0f));
}

App::~App()
{
    ImGui::SFML::Shutdown();
}

void App::Run()
{
    Update();

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
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code >= sf::Keyboard::Num1 && event.key.code <= sf::Keyboard::Num9)
                {
                    m_SelectedEntityIdx = event.key.code - (int)sf::Keyboard::Num1;
                }
                else if (event.key.code <= sf::Keyboard::Escape)
                {
                    m_SelectedEntityIdx = -1;
                }
                else if (event.key.code >= sf::Keyboard::Left && event.key.code <= sf::Keyboard::Down)
                {
                    if (m_SelectedEntityIdx != -1)
                    {
                        m_SystemTransform.Translate(m_Registry,
                            *m_Entities[m_SelectedEntityIdx],
                            glm::vec3(
                                (event.key.code <= sf::Keyboard::Right ? 1 + 2 * (event.key.code - sf::Keyboard::Left - 1) : 0) * 10.0f,
                                (event.key.code >= sf::Keyboard::Up ? -1 + -2 * (event.key.code - sf::Keyboard::Up - 1) : 0) * 10.0f,
                                0.0f));
                        m_SystemRender.Update(m_Registry);
                    }
                }
            }
        }

        UpdateInterface();

        m_Window.clear(m_ClearColor);
        Render();

        m_DeltaClock.restart();
	}
}

void App::UpdateInterface()
{
    ImGui::SFML::Update(m_Window, m_DeltaClock.getElapsedTime());

    if (ImGui::Begin("Info"))
    {
        ImGui::Text("Active %d", m_SelectedEntityIdx);

        int renderMethod = m_GUIRenderMethod;
        ImGui::RadioButton("Painter Algorithm", &renderMethod, (int)BaseRenderSystem::RenderMethod::Painter);
        ImGui::RadioButton("ZBuffer", &renderMethod, (int)BaseRenderSystem::RenderMethod::ZBuffer);
        if (m_GUIRenderMethod != renderMethod)
        {
            m_GUIRenderMethod = renderMethod;

            m_SystemRender.ChangeRenderMethod(m_Registry, (BaseRenderSystem::RenderMethod)m_GUIRenderMethod);
        }
    }
    ImGui::End();
}

void App::Update()
{
    m_SystemRender.Update(m_Registry);
}

void App::Render()
{
    m_SystemRender.Render(m_Registry, m_Window);
    ImGui::SFML::Render(m_Window);
    m_Window.display();
}
