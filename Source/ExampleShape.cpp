#include "ExampleShape.hpp"

#include <glm/glm.hpp>
#include <vector>
#include "Component/SurfaceComponent.hpp"

std::vector<SurfaceComponent> Shape3DExample()
{
	return std::move(std::vector<SurfaceComponent>({
		// Left
		SurfaceComponent(
			glm::vec3(0.0f, 100.0f, 0.0f),
			glm::vec3(-50.0f, 0.0f, -50.0f),
			glm::vec3(-50.0f, 0.0f, 50.0f)),
		// Front
		SurfaceComponent(
			glm::vec3(0.0f, 100.0f, 0.0f),
			glm::vec3(-50.0f, 0.0f, 50.0f),
			glm::vec3(50.0f, 0.0f, 50.0f)),
		// Right
		SurfaceComponent(
			glm::vec3(0.0f, 100.0f, 0.0f),
			glm::vec3(50.0f, 0.0f, 50.0f),
			glm::vec3(50.0f, 0.0f, -50.0f)),
		// Behind
		SurfaceComponent(
			glm::vec3(0.0f, 100.0f, 0.0f),
			glm::vec3(50.0f, 0.0f, -50.0f),
			glm::vec3(-50.0f, 0.0f, -50.0f)),
		// Front Left Bottom
		SurfaceComponent(
			glm::vec3(-50.0f, 0.0f, 50.0f),
			glm::vec3(-50.0f, 0.0f, -50.0f),
			glm::vec3(50.0f, 0.0f, 50.0f)),
		// Behind Right Bottom
		SurfaceComponent(
			glm::vec3(50.0f, 0.0f, 50.0f),
			glm::vec3(-50.0f, 0.0f, -50.0f),
			glm::vec3(50.0f, 0.0f, -50.0f)),
		}));
}