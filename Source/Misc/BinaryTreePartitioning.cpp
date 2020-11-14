#include "BinaryTreePartitioning.hpp"

#include <glm/glm.hpp>
#include <algorithm>
#include <glm/glm.hpp>

#include "../Component/SurfaceComponent.hpp"
#include "../Core/Camera.hpp"
#include "../Util/Logger.hpp"

static float LineIntersection(glm::vec3& from, glm::vec3& to, glm::vec3& arbitraryPointOnPlane, glm::vec3& normal)
{
	return glm::dot(arbitraryPointOnPlane - from, normal) / glm::dot((to - from), normal);
}

static glm::vec3 LineParametric(glm::vec3& from, glm::vec3& to, float percentage)
{
	return glm::vec3(
		from[0] + (percentage * (to[0] - from[0])),
		from[1] + (percentage * (to[1] - from[1])),
		from[2] + (percentage * (to[2] - from[2]))
	);
}

BinaryTreePartitioning::BinaryTreePartitioning(std::vector<SurfaceComponent>& surfaces) :
	m_NodeLeft(nullptr),
	m_NodeRight(nullptr)
{
	// ???
	//Camera camera;

	assert(m_Surfaces.size() == 0 && "BSP with 0 surface");
	if (m_Surfaces.size() == 1)
	{
		return;
	}
	else
	{
		// First surface as the reference surface
		SurfaceComponent& referenceSurface = *surfaces.begin();
		m_Surfaces.push_back(referenceSurface);
		glm::vec3 normal = glm::cross(
			referenceSurface.m_Vertices[1] - referenceSurface.m_Vertices[0],
			referenceSurface.m_Vertices[2] - referenceSurface.m_Vertices[1]);

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
				dotProductResults[ithVertex].m_Dot = glm::dot(normal, it->m_Vertices[ithVertex] - referenceSurface.m_Vertices[0]);
			}
			std::sort(
				dotProductResults.begin(),
				dotProductResults.end(),
				[](DotResult& dotProductResults, DotResult& otherDotProductResults)
				{
					return dotProductResults.m_Dot < otherDotProductResults.m_Dot;
				}
			);

			// # Cases
			// Coplanar and front
			if (dotProductResults[0].m_Dot >= 0.0f && dotProductResults[1].m_Dot >= 0.0f && dotProductResults[2].m_Dot >= 0.0f)
			{
				frontSideSurfaces.push_back(*it);
			}
			// Back
			else if (dotProductResults[0].m_Dot < 0.0f && dotProductResults[1].m_Dot <= 0.0f && dotProductResults[2].m_Dot <= 0.0f)
			{
				backSideSurfaces.push_back(*it);
			}
			// TODO
			// Fix the circular painter algorithm
			// The rest of the control flows create a circular surface
			// Intersect
			else if (dotProductResults[0].m_Dot < 0.0f && dotProductResults[1].m_Dot < 0.0f && dotProductResults[2].m_Dot > 0.0f)
			{
				float acT = LineIntersection(
					it->m_Vertices[dotProductResults[0].m_Idx],
					it->m_Vertices[dotProductResults[2].m_Idx],
					referenceSurface.m_Vertices[0],
					normal);
				float bcT = LineIntersection(
					it->m_Vertices[dotProductResults[1].m_Idx],
					it->m_Vertices[dotProductResults[2].m_Idx],
					referenceSurface.m_Vertices[0],
					normal);
				glm::vec3 d = LineParametric(it->m_Vertices[dotProductResults[0].m_Idx], it->m_Vertices[dotProductResults[2].m_Idx], acT);
				glm::vec3 e = LineParametric(it->m_Vertices[dotProductResults[1].m_Idx], it->m_Vertices[dotProductResults[2].m_Idx], bcT);

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
			else if (dotProductResults[0].m_Dot < 0.0f && dotProductResults[1].m_Dot > 0.0f && dotProductResults[2].m_Dot > 0.0f)
			{
				float bcT = LineIntersection(
					it->m_Vertices[dotProductResults[1].m_Idx],
					it->m_Vertices[dotProductResults[0].m_Idx],
					referenceSurface.m_Vertices[0],
					normal);
				float acT = LineIntersection(
					it->m_Vertices[dotProductResults[2].m_Idx],
					it->m_Vertices[dotProductResults[0].m_Idx],
					referenceSurface.m_Vertices[0],
					normal);

				glm::vec3 d = LineParametric(it->m_Vertices[dotProductResults[2].m_Idx], it->m_Vertices[dotProductResults[0].m_Idx], acT);
				glm::vec3 e = LineParametric(it->m_Vertices[dotProductResults[1].m_Idx], it->m_Vertices[dotProductResults[0].m_Idx], bcT);

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
			else if (dotProductResults[0].m_Dot < 0.0f && dotProductResults[1].m_Dot == 0.0f && dotProductResults[2].m_Dot > 0.0f)
			{
				float acT = LineIntersection(
					it->m_Vertices[dotProductResults[0].m_Idx],
					it->m_Vertices[dotProductResults[2].m_Idx],
					referenceSurface.m_Vertices[0],
					normal);

				glm::vec3 d = LineParametric(it->m_Vertices[dotProductResults[0].m_Idx], it->m_Vertices[dotProductResults[2].m_Idx], acT);

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
