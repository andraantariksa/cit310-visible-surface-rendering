#include "BinaryTreePartitioning.hpp"

#include <glm/glm.hpp>
#include <algorithm>
#include <glm/glm.hpp>
#include <sstream>
#include <graphviz/gvc.h>
#include <graphviz/cgraph.h>
#include <limits>

#include "../Component/SurfaceComponent.hpp"
#include "../Core/Camera.hpp"
#include "../Util/Logger.hpp"

static float LineIntersection(const glm::vec3& from, const glm::vec3& to, const glm::vec3& arbitraryPointOnPlane, const glm::vec3& normal)
{
	float denominator = glm::dot(to - from, normal);
	assert(denominator != 0.0f && "Uh oh cross(to - from, normal) is 0");
	return glm::dot(arbitraryPointOnPlane - from, normal) / denominator;
}

static glm::vec3 LineParametric(const glm::vec3& from, const glm::vec3& to, const float percentage)
{
	return glm::vec3(
		from.x + percentage * (to.x - from.x),
		from.y + percentage * (to.y - from.y),
		from.z + percentage * (to.z - from.z)
	);
}

template <typename T>
static bool eq(T a, T b) {
	T e = std::numeric_limits<T>::epsilon();
	return std::fabs(a - b) <= e;
}

template <typename T>
bool lt(T a, T b) {
	if (!eq(a, b))
	{
		return a < b;
	}
	return false;
}

template <typename T>
bool lte(T a, T b) {
	if (eq(a, b))
	{
		return true;
	}
	return a < b;
}

template <typename T>
bool gt(T a, T b) {
	if (!eq(a, b))
	{
		return a > b;
	}
	return false;
}

template <typename T>
bool gte(T a, T b) {
	if (eq(a, b))
	{
		return true;
	}
	return a > b;
}



BinaryTreePartitioning::BinaryTreePartitioning(const std::vector<SurfaceComponent>& surfaces)
{
	Construct(surfaces);
}

void BinaryTreePartitioning::Construct(const std::vector<SurfaceComponent>& surfaces)
{
	// ???
	//Camera camera;

	assert(surfaces.size() != 0 && "BSP with 0 surface");

	// First surface as the reference surface
	const SurfaceComponent& referenceSurface = surfaces[0];
	const glm::vec3& referenceVertex = referenceSurface.m_Vertices[0];
	m_Surfaces.push_back(referenceSurface);
	glm::vec3 referenceSurfaceNormal = glm::cross(
		referenceSurface.m_Vertices[1] - referenceSurface.m_Vertices[0],
		referenceSurface.m_Vertices[2] - referenceSurface.m_Vertices[1]);

	if (!(glm::dot(referenceSurfaceNormal, glm::vec3(0.0f, 0.0f, -1.0)) < 0.0f))
	{
		referenceSurfaceNormal = glm::cross(
			referenceSurface.m_Vertices[1] - referenceSurface.m_Vertices[2],
			referenceSurface.m_Vertices[0] - referenceSurface.m_Vertices[1]);
	}

	// To store the front and back surface
	std::vector<SurfaceComponent> frontSideSurfaces;
	std::vector<SurfaceComponent> backSideSurfaces;

	// Start from the second surface
	for (auto it = std::next(surfaces.begin()); it != surfaces.end(); ++it)
	{
		// Traverse on the polygon vertices (Because it's a triangle then the total vertex is 3)
		std::array<DotResult, 3> dotProductResults;
		for (size_t ithVertex = 0; ithVertex < it->m_Vertices.size(); ++ithVertex)
		{
			dotProductResults[ithVertex].m_Idx = ithVertex;
			dotProductResults[ithVertex].m_Dot = glm::dot(
				referenceSurfaceNormal,
				it->m_Vertices[ithVertex] - referenceVertex);
		}
		std::sort(
			dotProductResults.begin(),
			dotProductResults.end(),
			[](const DotResult& current, const DotResult& other)
			{
				return current.m_Dot < other.m_Dot;
			}
		);

		// # Cases
		// Coplanar and front
		if (gte(dotProductResults[0].m_Dot, 0.0f) && gte(dotProductResults[1].m_Dot, 0.0f) && gte(dotProductResults[2].m_Dot, 0.0f))
		{
			frontSideSurfaces.push_back(*it);
		}
		// Back
		else if (lt(dotProductResults[0].m_Dot, 0.0f) && lte(dotProductResults[1].m_Dot, 0.0f) && lte(dotProductResults[2].m_Dot, 0.0f))
		{
			backSideSurfaces.push_back(*it);
		}
		// TODO
		// Fix the circular painter algorithm
		// The rest of the control flows create a circular surface
		// Intersect
		else if (lt(dotProductResults[0].m_Dot, 0.0f) && lt(dotProductResults[1].m_Dot, 0.0f) && gt(dotProductResults[2].m_Dot, 0.0f))
		{
			glm::vec3 dA = it->m_Vertices[dotProductResults[0].m_Idx];
			glm::vec3 dB = it->m_Vertices[dotProductResults[1].m_Idx];
			glm::vec3 dC = it->m_Vertices[dotProductResults[2].m_Idx];
			glm::vec3 d = LineParametric(
				dA,
				dC,
				LineIntersection(
					dA,
					dC,
					referenceVertex,
					referenceSurfaceNormal));
			glm::vec3 e = LineParametric(
				dB,
				dC,
				LineIntersection(
					dB,
					dC,
					referenceVertex,
					referenceSurfaceNormal));

			backSideSurfaces.push_back(
				SurfaceComponent(
					it->m_Vertices[dotProductResults[0].m_Idx],
					d,
					it->m_Vertices[dotProductResults[1].m_Idx],
					it->m_Color
				)
			);
			backSideSurfaces.push_back(
				SurfaceComponent(
					it->m_Vertices[dotProductResults[1].m_Idx],
					d,
					e,
					it->m_Color
				)
			);
			frontSideSurfaces.push_back(
				SurfaceComponent(
					it->m_Vertices[dotProductResults[2].m_Idx],
					e,
					d,
					it->m_Color
				)
			);
		}
		else if (lt(dotProductResults[0].m_Dot, 0.0f) && gt(dotProductResults[1].m_Dot, 0.0f) && gt(dotProductResults[2].m_Dot, 0.0f))
		{
			glm::vec3 dA = it->m_Vertices[dotProductResults[0].m_Idx];
			glm::vec3 dB = it->m_Vertices[dotProductResults[1].m_Idx];
			glm::vec3 dC = it->m_Vertices[dotProductResults[2].m_Idx];

			glm::vec3 d = LineParametric(
				dA,
				dC,
				LineIntersection(
					dA,
					dC,
					referenceVertex,
					referenceSurfaceNormal));
			glm::vec3 e = LineParametric(
				dA,
				dB,
				LineIntersection(
					dA,
					dB,
					referenceVertex,
					referenceSurfaceNormal));

			frontSideSurfaces.push_back(
				SurfaceComponent(
					it->m_Vertices[dotProductResults[2].m_Idx],
					d,
					it->m_Vertices[dotProductResults[1].m_Idx],
					it->m_Color
				)
			);
			frontSideSurfaces.push_back(
				SurfaceComponent(
					it->m_Vertices[dotProductResults[1].m_Idx],
					d,
					e,
					it->m_Color
				)
			);
			backSideSurfaces.push_back(
				SurfaceComponent(
					it->m_Vertices[dotProductResults[0].m_Idx],
					e,
					d,
					it->m_Color
				)
			);
		}
		else if (lt(dotProductResults[0].m_Dot, 0.0f) && eq(dotProductResults[1].m_Dot, 0.0f) && gt(dotProductResults[2].m_Dot, 0.0f))
		{
			glm::vec3 dA = it->m_Vertices[dotProductResults[0].m_Idx];
			glm::vec3 dB = it->m_Vertices[dotProductResults[1].m_Idx];
			glm::vec3 dC = it->m_Vertices[dotProductResults[2].m_Idx];

			glm::vec3 d = LineParametric(
				dA,
				dC,
				LineIntersection(
					dA,
					dC,
					referenceVertex,
					referenceSurfaceNormal));

			backSideSurfaces.push_back(
				SurfaceComponent(
					it->m_Vertices[dotProductResults[0].m_Idx],
					it->m_Vertices[dotProductResults[1].m_Idx],
					d,
					it->m_Color
				)
			);
			frontSideSurfaces.push_back(
				SurfaceComponent(
					d,
					it->m_Vertices[dotProductResults[1].m_Idx],
					it->m_Vertices[dotProductResults[2].m_Idx],
					it->m_Color
				)
			);
		}

		if (!backSideSurfaces.empty())
		{
			// Left for back
			m_NodeLeft = std::make_unique<BinaryTreePartitioning>(backSideSurfaces);
		}

		if (!frontSideSurfaces.empty())
		{
			// Right for front
			m_NodeRight = std::make_unique<BinaryTreePartitioning>(frontSideSurfaces);
		}
	}
}

void BinaryTreePartitioning::Traverse(std::function<void(std::vector<SurfaceComponent>&)>& f)
{
	if (m_NodeLeft)
	{
		m_NodeLeft->Traverse(f);
	}
	f(m_Surfaces);
	if (m_NodeRight)
	{
		m_NodeRight->Traverse(f);
	}
}

Agnode_t* BinaryTreePartitioning::GetCGraphTree(Agraph_t* graph)
{
	INFO << "Graph\n";
	Agnode_t* currentNode;

	std::stringstream ss;
	for (auto& surface : m_Surfaces)
	{
		ss << surface.m_Color.r << ", " << surface.m_Color.g << ", " << surface.m_Color.b << " | ";
	}
	currentNode = agnode(graph, (char*)ss.str().c_str(), true);
	assert(currentNode);

	if (m_NodeLeft)
	{
		Agnode_t* leftNode = m_NodeLeft->GetCGraphTree(graph);

		assert(agedge(graph, currentNode, leftNode, (char*)"AAA", true));
	}

	if (m_NodeRight)
	{
		Agnode_t* rightNode = m_NodeRight->GetCGraphTree(graph);

		assert(agedge(graph, currentNode, rightNode, (char*)"aaa", true));
	}

	return currentNode;
}

void BinaryTreePartitioning::Clear()
{
	m_NodeLeft.reset();
	m_NodeRight.reset();
	m_Surfaces.clear();
}

