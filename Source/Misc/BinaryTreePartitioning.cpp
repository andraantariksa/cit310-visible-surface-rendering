#include "BinaryTreePartitioning.hpp"

#include <glm/glm.hpp>
#include <algorithm>
#include "../Component/SurfaceComponent.hpp"

BinaryTreePartitioning::BinaryTreePartitioning(std::vector<SurfaceComponent>& surfaces)
{
	if (m_Surfaces.size() == 1)
	{
		return;
	}
	else
	{
		// First surface as the reference surface
		auto& it = m_Surfaces.cbegin();
		const SurfaceComponent& referenceSurface = (*++it);
		m_Surfaces.push_back(referenceSurface);
		glm::vec3 normal = glm::cross(
			referenceSurface.m_Vertices[1] - referenceSurface.m_Vertices[0],
			referenceSurface.m_Vertices[2] - referenceSurface.m_Vertices[1]);
		
		// To store the front and back surface
		std::vector<SurfaceComponent> frontSideSurfaces;
		std::vector<SurfaceComponent> backSideSurfaces;

		// Start from the second surface
		for (; it != m_Surfaces.cend(); ++it)
		{
			// Traverse on the polygon vertices (Because it's a triangle then the total vertex is 3)
			std::array<float, 3> dotProductResults;
			for (size_t ithVertex = 0; ithVertex < (*it).m_Vertices.size(); ++ithVertex)
			{
				dotProductResults[ithVertex] = glm::dot(normal, (*it).m_Vertices[ithVertex] - referenceSurface.m_Vertices[0]);
			}
			std::sort(dotProductResults.begin(), dotProductResults.end());
			
			// # Cases
			// Coplanar and front
			if (dotProductResults[0] >= 0.0f && dotProductResults[1] >= 0.0f && dotProductResults[2] >= 0.0f)
			{
				frontSideSurfaces.push_back(*it);
			}
			// Back
			else if (dotProductResults[0] < 0.0f && dotProductResults[1] <= 0.0f && dotProductResults[2] <= 0.0f)
			{
				backSideSurfaces.push_back(*it);
			}
			// Others
			else if (dotProductResults[0] < 0.0f && dotProductResults[1] < 0.0f && dotProductResults[2] > 0.0f)
			{

			}
		}

		if (backSideSurfaces.size() > 0)
		{
			// Left for back
			m_NodeLeft = std::make_unique<BinaryTreePartitioning>(backSideSurfaces);
		}

		if (frontSideSurfaces.size() > 0)
		{
			// Right for front
			m_NodeRight = std::make_unique<BinaryTreePartitioning>(frontSideSurfaces);;
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
