#include "ExampleShape.hpp"

#include <glm/glm.hpp>
#include <vector>
#include "Component/SurfaceComponent.hpp"

static bool state = false;

// TODO: Fix the color to use a different color
std::vector<SurfaceComponent> Shape3DExample()
{
	state = !state;
	return std::move(std::vector<SurfaceComponent>({
		// Left
		SurfaceComponent(
			glm::vec3(0.0f, 100.0f, 0.0f),
			glm::vec3(-50.0f, 0.0f, -50.0f),
			glm::vec3(-50.0f, 0.0f, 50.0f),
			state ? sf::Color::Black : sf::Color::White),
		// Front
		SurfaceComponent(
			glm::vec3(0.0f, 100.0f, 0.0f),
			glm::vec3(-50.0f, 0.0f, 50.0f),
			glm::vec3(50.0f, 0.0f, 50.0f),
			state ? sf::Color::Green : sf::Color::Yellow),
		// Right
		SurfaceComponent(
			glm::vec3(0.0f, 100.0f, 0.0f),
			glm::vec3(50.0f, 0.0f, 50.0f),
			glm::vec3(50.0f, 0.0f, -50.0f),
			state ? sf::Color::Red : sf::Color::Magenta),
		// Behind
		SurfaceComponent(
			glm::vec3(0.0f, 100.0f, 0.0f),
			glm::vec3(50.0f, 0.0f, -50.0f),
			glm::vec3(-50.0f, 0.0f, -50.0f),
			state ? sf::Color::Yellow : sf::Color::Cyan),
		// Front Left Bottom
		SurfaceComponent(
			glm::vec3(-50.0f, 0.0f, 50.0f),
			glm::vec3(-50.0f, 0.0f, -50.0f),
			glm::vec3(50.0f, 0.0f, 50.0f),
			state ? sf::Color::Blue : sf::Color::Blue),
		// Behind Right Bottom
		SurfaceComponent(
			glm::vec3(50.0f, 0.0f, 50.0f),
			glm::vec3(-50.0f, 0.0f, -50.0f),
			glm::vec3(50.0f, 0.0f, -50.0f),
			state ? sf::Color::Blue : sf::Color::Blue),
		}));
}