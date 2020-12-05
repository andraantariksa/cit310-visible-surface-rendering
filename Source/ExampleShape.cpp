#include "ExampleShape.hpp"

#include <glm/glm.hpp>
#include <vector>
#include "Component/Shape3DComponent.hpp"
#include "Component/Surface3DComponent.hpp"

static bool state = false;

Shape3DComponent Shape3DExample()
{
	state = !state;
	return std::move(Shape3DComponent(
		{
			glm::dvec3(0.0, 200.0, 0.0), // Top
			glm::dvec3(-100.0, 0.0, -100.0), // Left Behind
			glm::dvec3(-100.0, 0.0, 100.0), // Left Front
			glm::dvec3(100.0, 0.0, -100.0), // Right Behind
			glm::dvec3(100.0, 0.0, 100.0), // Right Front
		},
		{
		//// Left
		Surface3DComponent(
			std::array<std::size_t, 3>({ 0, 1, 2 }),
			state ? sf::Color::Black : sf::Color::White),
		// Front
		Surface3DComponent(
			std::array<std::size_t, 3>({ 0, 2, 4 }),
			state ? sf::Color::Green : sf::Color(0, 153, 230)),
		//// Right
		Surface3DComponent(
			std::array<std::size_t, 3>({ 0, 4, 3 }),
			state ? sf::Color::Red : sf::Color::Magenta),
		//// Behind
		Surface3DComponent(
			std::array<std::size_t, 3>({ 0, 3, 1 }),
			state ? sf::Color::Yellow : sf::Color(115, 130, 230)),
		//// Front Left Bottom
		Surface3DComponent(
			std::array<std::size_t, 3>({ 2, 1, 4 }),
			state ? sf::Color(235, 64, 52) : sf::Color(107, 19, 13)),
		//// Behind Right Bottom
		Surface3DComponent(
			std::array<std::size_t, 3>({ 4, 1, 3 }),
			state ? sf::Color(225, 191, 255) : sf::Color::Blue)
		}));
}