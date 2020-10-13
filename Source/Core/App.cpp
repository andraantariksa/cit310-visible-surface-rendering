#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include "App.hpp"
#include "../Component/SphereComponent.hpp"
#include "../Tags/SphereTags.hpp"

App::App():
	window(sf::VideoMode(640, 480), "ImGui + SFML = <3")
{
	window.setFramerateLimit(30);
    ImGui::SFML::Init(window);

    const auto sphere1 = registry.create();
    // TODO
    //registry.emplace<SphereTag>(sphere1);
    //registry.emplace<SphereComponent>(sphere1, 1);
}

App::~App()
{
    ImGui::SFML::Shutdown();
}

void App::Run()
{
	while (window.isOpen())
	{
        window.clear(sf::Color::White);
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();
        }

        UpdateInterface();

        Render();

        deltaClock.restart();
	}
}

void App::UpdateInterface()
{
    ImGui::SFML::Update(window, deltaClock.getElapsedTime());

    ImGui::Begin("Hello, world!");
    ImGui::Button("Look at this pretty button");
    ImGui::End();

    ImGui::SFML::Render(window);
}

void App::Render()
{
    window.display();
}
