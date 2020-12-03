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
		//// Left
		SurfaceComponent(
			glm::dvec3(0.0, 200.0, 0.0),
			glm::dvec3(-100.0, 0.0, -100.0),
			glm::dvec3(-100.0, 0.0, 100.0),
			state ? sf::Color::Black : sf::Color::White),
		// Front
		SurfaceComponent(
			glm::dvec3(0.0, 200.0, 0.0),
			glm::dvec3(-100.0, 0.0, 100.0),
			glm::dvec3(100.0, 0.0, 100.0),
			state ? sf::Color::Green : sf::Color(0, 153, 230)),
		// Right
		SurfaceComponent(
			glm::dvec3(0.0, 200.0, 0.0),
			glm::dvec3(100.0, 0.0, 100.0),
			glm::dvec3(100.0, 0.0, -100.0),
			state ? sf::Color::Red : sf::Color::Magenta),
		// Behind
		SurfaceComponent(
			glm::dvec3(0.0, 200.0, 0.0),
			glm::dvec3(100.0, 0.0, -100.0),
			glm::dvec3(-100.0, 0.0, -100.0),
			state ? sf::Color::Yellow : sf::Color(115, 130, 230)),
		// Front Left Bottom
		SurfaceComponent(
			glm::dvec3(-100.0, 0.0, 100.0),
			glm::dvec3(-100.0, 0.0, -100.0),
			glm::dvec3(100.0, 0.0, 100.0),
			state ? sf::Color(235, 64, 52) : sf::Color(107, 19, 13)),
		// Behind Right Bottom
		SurfaceComponent(
			glm::dvec3(100.0, 0.0, 100.0),
			glm::dvec3(-100.0, 0.0, -100.0),
			glm::dvec3(100.0, 0.0, -100.0),
			state ? sf::Color(225, 191, 255) : sf::Color::Blue),
		}));
}