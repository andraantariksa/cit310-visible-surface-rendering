#ifndef __MISC_BINARYTREEPARTITIONING_HPP
#define __MISC_BINARYTREEPARTITIONING_HPP

#include <functional>
#include <memory>
#include <vector>
#include <utility>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/fileformats/GraphIO.h>

#include "../Component/Triangle3DComponent.hpp"

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

	std::unique_ptr<Triangle3DComponent> m_Surface;

	std::unique_ptr<BinaryTreePartitioning> m_NodeLeft;

	std::unique_ptr<BinaryTreePartitioning> m_NodeRight;

	BinaryTreePartitioning() = default;
	~BinaryTreePartitioning() = default;

	BinaryTreePartitioning(const std::vector<Triangle3DComponent>& surfaces);
	void Construct(const std::vector<Triangle3DComponent>& surfaces);
	void Clear();

	void Traverse(std::function<void(Triangle3DComponent&)>& f);
	void GetTreeGraph(
		ogdf::Graph& graph,
		ogdf::GraphAttributes& graphAtt,
		ogdf::node* parentNode = nullptr,
		Dir dir = Dir::None);
};

#endif
