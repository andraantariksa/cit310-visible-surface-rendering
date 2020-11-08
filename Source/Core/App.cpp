#include "App.hpp"

#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <glm/glm.hpp>

#include "../Macro.hpp"
#include "../Component/SphereComponent.hpp"
#include "../Util/Logger.hpp"

App::App():
	m_Window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), APP_NAME, sf::Style::Titlebar | sf::Style::Close),
    m_ClearColor(96, 96, 96),
    m_GUISphereRadius(200.0f),
    m_GUISphereTranslateX(0.0f),
    m_GUISphereTranslateY(0.0f),
    m_GUISphereTranslateZ(0.0f),
    m_GUISphereLongitude(5),
    m_GUISphereLatitude(5),
    m_GUISphereRotationAxis(0),
    m_GUISphereRotationDegree(0.0f),
    m_GUISphereBackfaceCulling(true),
    m_GUIVanishingPointZ(-500.0f),
    m_EntitySphere(m_Registry.create()),
    m_GUISurfaceNormalValid(false)
{
	m_Window.setFramerateLimit(15);
    ImGui::SFML::Init(m_Window);
    ImGui::GetIO().IniFilename = nullptr;

    m_Registry.emplace<TransformComponent>(m_EntitySphere);
    m_Registry.emplace<SphereComponent>(m_EntitySphere, m_GUISphereRadius, m_GUISphereLongitude, m_GUISphereLatitude);
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
            else if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Right)
            {
                rightClick = true;
            }
        }

        if (rightClick)
        {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(m_Window);
            std::optional<glm::vec3> normalClickedSurface = m_SystemRender.GetSphereSurfaceNormal(
                m_Registry,
                glm::vec2((float)mousePosition.x, (float)mousePosition.y)
            );
            if (normalClickedSurface.has_value())
            {
                glm::vec3 normal_surface = glm::normalize(normalClickedSurface.value());
                m_GUISurfaceNormal[0] = normal_surface.x;
                m_GUISurfaceNormal[1] = normal_surface.y;
                m_GUISurfaceNormal[2] = normal_surface.z;
                m_GUISurfaceNormalValid = true;
            }
            else
            {
                m_GUISurfaceNormalValid = false;
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

    ImGui::SetNextWindowSize(ImVec2(430.0f, 220.0f));
    if(ImGui::Begin("Sphere Editor", (bool*)0, ImGuiWindowFlags_NoResize))
    {
        if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
        {
            if (ImGui::BeginTabItem("Properties"))
            {
                ImGui::InputFloat("Radius", &m_GUISphereRadius, 0.01f, 1.0f);
                if (m_GUISphereRadius < 0.0f)
                {
                    m_GUISphereRadius = 0.0f;
                }

                ImGui::InputInt("Latitude", &m_GUISphereLatitude);
                if (m_GUISphereLatitude < 3)
                {
                    m_GUISphereLatitude = 3;
                }

                ImGui::InputInt("Longitude", &m_GUISphereLongitude);
                if (m_GUISphereLongitude < 3)
                {
                    m_GUISphereLongitude = 3;
                }

                // need to check if not zero somehow
                ImGui::InputFloat("Vanishing Point Z", &m_GUIVanishingPointZ, 0.01f, 1.0f);
                ImGui::Checkbox("Backface Culling", &m_GUISphereBackfaceCulling);

                if (ImGui::Button("Update"))
                {
                    SphereComponent& sphereComponent = m_Registry.get<SphereComponent>(m_EntitySphere);
                    sphereComponent.m_R = m_GUISphereRadius;
                    sphereComponent.m_NLatitude = m_GUISphereLatitude;
                    sphereComponent.m_NLongitude = m_GUISphereLongitude;
                    sphereComponent.m_IsCullBackface = m_GUISphereBackfaceCulling;
                    sphereComponent.RegenerateVertices();

                    // need optimized
                    m_SystemRender.ResetMatrix(m_GUIVanishingPointZ);
                }
                ImGui::PushItemWidth(20);
                if (ImGui::Button("Reset"))
                {
                    m_GUIVanishingPointZ = -500.0f;
                    m_SystemRender.ResetTransform(m_Registry);
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Translation"))
            {
                ImGui::InputFloat("X", &m_GUISphereTranslateX, 0.01f, 1.0f);
                ImGui::InputFloat("Y", &m_GUISphereTranslateY, 0.01f, 1.0f);
                ImGui::InputFloat("Z", &m_GUISphereTranslateZ, 0.01f, 1.0f);
                if (ImGui::Button("Translate"))
                {
                    TransformComponent& transformComponent = m_Registry.get<TransformComponent>(m_EntitySphere);
                    transformComponent.Translate(glm::vec3(m_GUISphereTranslateX, m_GUISphereTranslateY, m_GUISphereTranslateZ));

                    // Reset the value
                    /*m_GUISphereTranslateX = 0.0f;
                    m_GUISphereTranslateY = 0.0f;
                    m_GUISphereTranslateZ = 0.0f;*/
                }

                ImGui::EndTabItem();
            }


            if (ImGui::BeginTabItem("Rotation"))
            {
                ImGui::InputFloat("Degree", &m_GUISphereRotationDegree, 0.01f, 10.0f);
                ImGui::RadioButton("X", &m_GUISphereRotationAxis, 0);
                ImGui::RadioButton("Y", &m_GUISphereRotationAxis, 1);
                ImGui::RadioButton("Z", &m_GUISphereRotationAxis, 2);
                if (ImGui::Button("Rotate"))
                {
                    TransformComponent& transformComponent = m_Registry.get<TransformComponent>(m_EntitySphere);

                    glm::vec3 rotationAxis(0.0f);
                    rotationAxis[m_GUISphereRotationAxis] = 1.0f;
                    transformComponent.Rotate(m_GUISphereRotationDegree, rotationAxis);

                    // (Don't) Reset the value
                    //m_GUISphereRotationDegree = 0.0f;
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Misc"))
            {
                ImGui::TextWrapped("Press right click on the sphere surface to get the surface normal");
                if (m_GUISurfaceNormalValid)
                {
                    ImGui::InputFloat3("Normal", m_GUISurfaceNormal, 3, ImGuiInputTextFlags_ReadOnly);
                }
                else
                {
                    ImGui::TextWrapped("Invalid surface");
                }

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }
    ImGui::End();
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
