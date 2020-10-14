#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <glm/glm.hpp>

#include "App.hpp"
#include "../Component/PositionComponent.hpp"
#include "../Component/SphereComponent.hpp"

App::App():
	m_Window(sf::VideoMode(600, 600), "Spheros", sf::Style::Titlebar | sf::Style::Close),
    m_ClearColor(96.0f, 96.0f, 96.0f),
    m_GUISphereRadius(1.0f)
{
	m_Window.setFramerateLimit(20);
    ImGui::SFML::Init(m_Window);

    const entt::entity sphere1 = m_Registry.create();
    m_Registry.emplace<PositionComponent>(sphere1, glm::vec3(0.0f, 0.0f, 0.0f));
    m_Registry.emplace<SphereComponent>(sphere1, 200.0f);
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
        while (m_Window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
                m_Window.close();
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

    if(ImGui::Begin("Sphere Editor"))
    {
        if (ImGui::TreeNode("Properties"))
        {
            ImGui::InputFloat("Radius", &m_GUISphereRadius, 0.01f, 1.0f, 2);
            ImGui::InputInt("Longitude", &m_GUISphereLongitude);
            ImGui::InputInt("Latitude", &m_GUISphereLatitude);
            ImGui::Button("Update");

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Translation"))
        {
            ImGui::InputFloat("X", &m_GUISpherePositionX, 0.01f, 1.0f, 2);
            ImGui::InputFloat("Y", &m_GUISpherePositionY, 0.01f, 1.0f, 2);
            ImGui::InputFloat("Z", &m_GUISpherePositionZ, 0.01f, 1.0f, 2);
            ImGui::Button("Move");

            ImGui::TreePop();
        }


        if (ImGui::TreeNode("Rotation"))
        {
            ImGui::InputFloat("X", &m_GUISpherePositionX, 0.01f, 1.0f, 2);
            ImGui::InputFloat("Y", &m_GUISpherePositionY, 0.01f, 1.0f, 2);
            ImGui::InputFloat("Z", &m_GUISpherePositionZ, 0.01f, 1.0f, 2);
            ImGui::Button("Rotate");

            ImGui::TreePop();
        }
    }
    ImGui::End();
}

void App::Update()
{
    m_SystemTransform.Update(m_Registry, nullptr);
}

void App::Render()
{
    m_SystemRender.Render(m_Registry, m_Window);
    ImGui::SFML::Render(m_Window);
    m_Window.display();
}
