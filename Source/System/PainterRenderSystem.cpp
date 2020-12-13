#include "PainterRenderSystem.hpp"

#include <glm/glm.hpp>
#include <tuple>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/SugiyamaLayout.h>
#include <sstream>
#include <cstdio>
#include <fstream>

#include "BaseRenderSystem.hpp"
#include "../Component/TransformComponent.hpp"
#include "../Component/Surface3DComponent.hpp"
#include "../Component/Triangle2DComponent.hpp"
#include "../Component/Shape3DComponent.hpp"
#include "../Misc/BinaryTreePartitioning.hpp"
#include "../Util/Logger.hpp"
#include "../Misc/EdgeBucket.hpp"

void PainterRenderSystem::Update(entt::registry& registry, BaseRenderSystem& baseRenderSystem, std::vector<Triangle3DComponent>& surfacesVCS)
{
	m_CachedTriangle2D.clear();
	m_BinaryPartitioningTree.Clear();

	m_BinaryPartitioningTree.Construct(surfacesVCS);

	std::function<void(Triangle3DComponent&)> traverseLambda = [&](Triangle3DComponent& surface) -> void {
		m_CachedTriangle2D.push_back(
			Triangle2DComponent(
				baseRenderSystem.TransformVCSToSCS(glm::dvec4(surface.m_Vertices[0], 1.0)),
				baseRenderSystem.TransformVCSToSCS(glm::dvec4(surface.m_Vertices[1], 1.0)),
				baseRenderSystem.TransformVCSToSCS(glm::dvec4(surface.m_Vertices[2], 1.0)),
				surface.m_Color)
		);
	};

	m_BinaryPartitioningTree.Traverse(traverseLambda);
}

void PainterRenderSystem::Render(entt::registry& registry, BaseRenderSystem& baseRenderSystem, sf::RenderWindow& window)
{
	sf::VertexArray vertArray(sf::Triangles);
	for (auto& surface : m_CachedTriangle2D)
	{
		vertArray.append(
			sf::Vertex(
				sf::Vector2f(surface.m_Vertices[0].x, surface.m_Vertices[0].y), surface.m_Color
			));
		vertArray.append(
			sf::Vertex(
				sf::Vector2f(surface.m_Vertices[1].x, surface.m_Vertices[1].y), surface.m_Color
			));
		vertArray.append(
			sf::Vertex(
				sf::Vector2f(surface.m_Vertices[2].x, surface.m_Vertices[2].y), surface.m_Color
			));
	}
	window.draw(vertArray);
}

void PainterRenderSystem::PrintBinaryPartitioningTree()
{
	ogdf::Graph graph;
	ogdf::GraphAttributes graphAtt(graph,
		ogdf::GraphAttributes::nodeGraphics |
		ogdf::GraphAttributes::edgeGraphics |
		ogdf::GraphAttributes::nodeLabel    |
		ogdf::GraphAttributes::edgeLabel    |
		ogdf::GraphAttributes::nodeStyle);

	m_BinaryPartitioningTree.GetTreeGraph(graph, graphAtt);

	ogdf::SugiyamaLayout sugiyamaLayout;
	sugiyamaLayout.setRanking(new ogdf::OptimalRanking);
	sugiyamaLayout.setCrossMin(new ogdf::MedianHeuristic);

	ogdf::OptimalHierarchyLayout* optimalHierarchyLayout = new ogdf::OptimalHierarchyLayout;
	optimalHierarchyLayout->layerDistance(30.0);
	optimalHierarchyLayout->nodeDistance(25.0);
	optimalHierarchyLayout->weightBalancing(0.8);
	sugiyamaLayout.setLayout(optimalHierarchyLayout);

	sugiyamaLayout.call(graphAtt);

	std::fstream file("tree.svg", std::ios::out);
	ogdf::GraphIO::SVGSettings svgSettings;
	svgSettings.fontSize(6);
	ogdf::GraphIO::drawSVG(graphAtt, file, svgSettings);
}
