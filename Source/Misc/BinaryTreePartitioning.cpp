#include "BinaryTreePartitioning.hpp"

#include <utility>
#include <glm/glm.hpp>
#include <algorithm>
#include <glm/glm.hpp>
#include <sstream>
#include <limits>
#include <type_traits>
#include <algorithm>
#include <ogdf/basic/Graph.h>
#include <ogdf/fileformats/GraphIO.h>

#include "../Component/Triangle3DComponent.hpp"
#include "../Core/Camera.hpp"
#include "../Util/Logger.hpp"

//template <typename T>
//static bool eq(T a, T b) {
//	return std::fabs(a - b) <= std::numeric_limits<T>::epsilon();
//}

template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
eq(T x, T y, int ulp=5)
{
	// the machine epsilon has to be scaled to the magnitude of the values used
	// and multiplied by the desired precision in ULPs (units in the last place)
	return std::fabs(x - y) <= std::numeric_limits<T>::epsilon() * std::fabs(x + y) * ulp
		// unless the result is subnormal
		|| std::fabs(x - y) < std::numeric_limits<T>::min();
}

template <typename T>
bool lt(T a, T b, T epsilon=std::numeric_limits<T>::epsilon()) {
	return (b - a) > ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

template <typename T>
bool lte(T a, T b) {
	return eq(a, b) || lt(a, b);
}

template <typename T>
bool gt(T a, T b, T epsilon = std::numeric_limits<T>::epsilon()) {
	return (a - b) > ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

template <typename T>
bool gte(T a, T b) {
	return eq(a, b) || gt(a, b);
}

static double LineIntersection(const glm::dvec3& from, const glm::dvec3& to, const glm::dvec3& arbitraryPointOnPlane, const glm::dvec3& normal)
{
	double denominator = glm::dot(to - from, normal);
#ifdef _DEBUG_BUILD
	assert(denominator != 0.0 && "Uh oh cross(to - from, normal) is 0");
#else
	if (denominator == 0.0)
	{
		throw std::runtime_error("denominator is 0.0. Error on LineIntersection");
	}
#endif
	double result = glm::dot(arbitraryPointOnPlane - from, normal) / denominator;
	result = glm::clamp(result, 0.0, 1.0);
#ifdef _DEBUG_BUILD
	assert(result >= 0.0 && result <= 1.0 && "Out of bound result");
#else
	if (!(result >= 0.0 && result <= 1.0))
	{
		throw std::runtime_error("Result is out of bound 0.0 <= result <= 1. Error on LineIntersection");
	}
#endif
	return result;
}

static glm::dvec3 LineParametric(const glm::dvec3& from, const glm::dvec3& to, const double percentage)
{
	return from + percentage * (to - from);
}

BinaryTreePartitioning::BinaryTreePartitioning(const std::vector<Triangle3DComponent>& surfaces)
{
	Construct(surfaces);
}

void BinaryTreePartitioning::Construct(const std::vector<Triangle3DComponent>& surfaces)
{
	//assert(surfaces.size() != 0 && "BSP with 0 surface");
	if (surfaces.size() == 0)
	{
		return;
	}

	// First surface as the reference surface
	const Triangle3DComponent referenceSurface = surfaces[0];
	const glm::dvec3& referenceVertex = referenceSurface.m_Vertices[0];
	m_Surfaces.push_back(referenceSurface);
	glm::dvec3 referenceSurfaceNormal = glm::cross(
		referenceSurface.m_Vertices[1] - referenceSurface.m_Vertices[0],
		referenceSurface.m_Vertices[2] - referenceSurface.m_Vertices[1]);

	if (!(glm::dot(referenceSurfaceNormal, Camera::Direction) < 0.0))
	{
		referenceSurfaceNormal = glm::cross(
			referenceSurface.m_Vertices[1] - referenceSurface.m_Vertices[2],
			referenceSurface.m_Vertices[0] - referenceSurface.m_Vertices[1]);
	}

	// To store the front and back surface
	std::vector<Triangle3DComponent> frontSideSurfaces;
	std::vector<Triangle3DComponent> backSideSurfaces;

	// Start from the second surface
	for (auto it = std::next(surfaces.begin()); it != surfaces.end(); ++it)
	{
		// Traverse on the polygon vertices (Because it's a triangle then the total vertex is 3)
		std::array<DotResult, 3> dotProductResults;
		for (size_t ithVertex = 0; ithVertex < 3; ++ithVertex)
		{
			dotProductResults[ithVertex].m_Idx = ithVertex;
			dotProductResults[ithVertex].m_Dot = glm::dot(
				referenceSurfaceNormal,
				it->m_Vertices[ithVertex] - referenceVertex);
			
			char sz[64];
			sprintf(sz, "%.7lf\n", dotProductResults[ithVertex].m_Dot);
			dotProductResults[ithVertex].m_Dot = atof(sz);
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
		if (gte(dotProductResults[0].m_Dot, 0.0) && gte(dotProductResults[1].m_Dot, 0.0) && gte(dotProductResults[2].m_Dot, 0.0))
		{
			frontSideSurfaces.push_back(*it);
		}
		// Back
		else if (lt(dotProductResults[0].m_Dot, 0.0) && lte(dotProductResults[1].m_Dot, 0.0) && lte(dotProductResults[2].m_Dot, 0.0))
		{
			backSideSurfaces.push_back(*it);
		}
		// TODO
		// Fix the circular painter algorithm
		// The rest of the control flows create a circular surface
		// Intersect
		else if (lt(dotProductResults[0].m_Dot, 0.0) && lt(dotProductResults[1].m_Dot, 0.0) && gt(dotProductResults[2].m_Dot, 0.0))
		{
			glm::dvec3 dA = it->m_Vertices[dotProductResults[0].m_Idx];
			glm::dvec3 dB = it->m_Vertices[dotProductResults[1].m_Idx];
			glm::dvec3 dC = it->m_Vertices[dotProductResults[2].m_Idx];
			glm::dvec3 d = LineParametric(
				dC,
				dA,
				LineIntersection(
					dC,
					dA,
					referenceVertex,
					referenceSurfaceNormal));
			glm::dvec3 e = LineParametric(
				dC,
				dB,
				LineIntersection(
					dC,
					dB,
					referenceVertex,
					referenceSurfaceNormal));

			backSideSurfaces.push_back(
				Triangle3DComponent(
					it->m_Vertices[dotProductResults[0].m_Idx],
					d,
					it->m_Vertices[dotProductResults[1].m_Idx],
					it->m_Color
				)
			);
			backSideSurfaces.push_back(
				Triangle3DComponent(
					it->m_Vertices[dotProductResults[1].m_Idx],
					d,
					e,
					it->m_Color
				)
			);
			frontSideSurfaces.push_back(
				Triangle3DComponent(
					it->m_Vertices[dotProductResults[2].m_Idx],
					e,
					d,
					it->m_Color
				)
			);
		}
		else if (lt(dotProductResults[0].m_Dot, 0.0) && gt(dotProductResults[1].m_Dot, 0.0) && gt(dotProductResults[2].m_Dot, 0.0))
		{
			glm::dvec3 dA = it->m_Vertices[dotProductResults[0].m_Idx];
			glm::dvec3 dB = it->m_Vertices[dotProductResults[1].m_Idx];
			glm::dvec3 dC = it->m_Vertices[dotProductResults[2].m_Idx];

			glm::dvec3 d = LineParametric(
				dC,
				dA,
				LineIntersection(
					dC,
					dA,
					referenceVertex,
					referenceSurfaceNormal));
			glm::dvec3 e = LineParametric(
				dB,
				dA,
				LineIntersection(
					dB,
					dA,
					referenceVertex,
					referenceSurfaceNormal));

			frontSideSurfaces.push_back(
				Triangle3DComponent(
					it->m_Vertices[dotProductResults[2].m_Idx],
					d,
					it->m_Vertices[dotProductResults[1].m_Idx],
					it->m_Color
				)
			);
			frontSideSurfaces.push_back(
				Triangle3DComponent(
					it->m_Vertices[dotProductResults[1].m_Idx],
					d,
					e,
					it->m_Color
				)
			);
			backSideSurfaces.push_back(
				Triangle3DComponent(
					it->m_Vertices[dotProductResults[0].m_Idx],
					e,
					d,
					it->m_Color
				)
			);
		}
		else if (lt(dotProductResults[0].m_Dot, 0.0) && eq(dotProductResults[1].m_Dot, 0.0) && gt(dotProductResults[2].m_Dot, 0.0))
		{
			glm::dvec3 dA = it->m_Vertices[dotProductResults[0].m_Idx];
			glm::dvec3 dB = it->m_Vertices[dotProductResults[1].m_Idx];
			glm::dvec3 dC = it->m_Vertices[dotProductResults[2].m_Idx];

			glm::dvec3 d = LineParametric(
				dC,
				dA,
				LineIntersection(
					dC,
					dA,
					referenceVertex,
					referenceSurfaceNormal));

			backSideSurfaces.push_back(
				Triangle3DComponent(
					it->m_Vertices[dotProductResults[0].m_Idx],
					it->m_Vertices[dotProductResults[1].m_Idx],
					d,
					it->m_Color
				)
			);
			frontSideSurfaces.push_back(
				Triangle3DComponent(
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

void BinaryTreePartitioning::Traverse(std::function<void(std::vector<Triangle3DComponent>&)>& f)
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

//void BinaryTreePartitioning::GetCGraphTree(ogdf::Graph& graph, ogdf::GraphAttributes& graphAtt, std::vector<std::pair<int, int>>& rank, ogdf::node* parentNode, BinaryTreePartitioning::Dir dir, int depth)
void BinaryTreePartitioning::GetTreeGraph(ogdf::Graph& graph, ogdf::GraphAttributes& graphAtt, ogdf::node* parentNode, BinaryTreePartitioning::Dir dir)
{
	/*std::stringstream ss;
	ogdf::node currentLevelNode = graph.newNode();
	graphAtt.strokeColor(currentLevelNode) = ogdf::Color(255, 0, 0);
	rank.push_back(std::make_pair(currentLevelNode->index(), depth));

	for (auto& surface : m_Surfaces)
	{
		ss << "a";

		ogdf::node currentNode = graph.newNode();
		graphAtt.fillColor(currentNode) = ogdf::Color(surface.m_Color.r, surface.m_Color.g, surface.m_Color.b);
		graphAtt.label(currentNode) = ss.str();
		rank.push_back(std::make_pair(currentNode->index(), depth));
		ss.clear();

		ogdf::edge edgeParentToCurrent = graph.newEdge(currentLevelNode, currentNode);
	}*/

	std::stringstream ss;
	ogdf::node currentLevelNode = graph.newNode();

	if (!m_Surfaces.empty())
	{
		auto& surface = m_Surfaces[0];
		ss << glm::to_string(surface.m_Vertices[0]) << "\n" << glm::to_string(surface.m_Vertices[1]) << "\n" << glm::to_string(surface.m_Vertices[2]);
		graphAtt.fillColor(currentLevelNode) = ogdf::Color(surface.m_Color.r, surface.m_Color.g, surface.m_Color.b);
		graphAtt.label(currentLevelNode) = ss.str();
		graphAtt.width(currentLevelNode) = graphAtt.width(currentLevelNode) * 3;
	}

	if (parentNode)
	{
		ogdf::edge edgeParentToCurrent = graph.newEdge(*parentNode, currentLevelNode);
		if (dir != BinaryTreePartitioning::Dir::None)
		{
			graphAtt.label(edgeParentToCurrent) = dir == BinaryTreePartitioning::Dir::Left ? "Left\0" : "Right";
		}
	}

	if (m_NodeLeft)
	{
		m_NodeLeft->GetTreeGraph(graph, graphAtt, &currentLevelNode, BinaryTreePartitioning::Dir::Left);
		//m_NodeLeft->GetCGraphTree(graph, graphAtt, rank, &currentLevelNode, BinaryTreePartitioning::Dir::Left, depth + 1);
	}

	if (m_NodeRight)
	{
		m_NodeRight->GetTreeGraph(graph, graphAtt, &currentLevelNode, BinaryTreePartitioning::Dir::Right);
		//m_NodeRight->GetCGraphTree(graph, graphAtt, rank, &currentLevelNode, BinaryTreePartitioning::Dir::Right, depth + 1);
	}
}

void BinaryTreePartitioning::Clear()
{
	m_NodeLeft.reset();
	m_NodeRight.reset();
	m_Surfaces.clear();
}
