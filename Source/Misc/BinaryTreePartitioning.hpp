#ifndef __MISC_BINARYTREEPARTITIONING_HPP
#define __MISC_BINARYTREEPARTITIONING_HPP

#include <functional>
#include <memory>
#include <vector>
#include <utility>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/fileformats/GraphIO.h>

#include "../Component/SurfaceComponent.hpp"

class DotResult
{
public:
	size_t m_Idx;
	double m_Dot;
};

class BinaryTreePartitioning
{
public:
	enum class Dir: char
	{
		None,
		Right,
		Left
	};

	std::vector<SurfaceComponent> m_Surfaces;

	std::unique_ptr<BinaryTreePartitioning> m_NodeLeft;

	std::unique_ptr<BinaryTreePartitioning> m_NodeRight;

	BinaryTreePartitioning() = default;
	~BinaryTreePartitioning() = default;

	BinaryTreePartitioning(const std::vector<SurfaceComponent>& surface);
	void Construct(const std::vector<SurfaceComponent>&surface);
	void Clear();

	void Traverse(std::function<void(std::vector<SurfaceComponent>&)>& f);
	void GetTreeGraph(ogdf::Graph& graph, ogdf::GraphAttributes& graphAtt, ogdf::node* parentNode = nullptr, Dir dir = Dir::None);
	//void GetCGraphTree(ogdf::Graph& graph, ogdf::GraphAttributes& graphAtt, std::vector<std::pair<int, int>>& rank, ogdf::node* parentNode=nullptr, Dir dir=Dir::None, int depth=0);
};

#endif
