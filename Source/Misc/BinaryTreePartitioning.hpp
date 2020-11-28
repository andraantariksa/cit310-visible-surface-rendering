#ifndef __MISC_BINARYTREEPARTITIONING_HPP
#define __MISC_BINARYTREEPARTITIONING_HPP

#include <functional>
#include <memory>
#include <vector>
#include <graphviz/gvc.h>
#include <graphviz/cgraph.h>

#include "../Component/SurfaceComponent.hpp"

class DotResult
{
public:
	size_t m_Idx;
	float m_Dot;
};

class BinaryTreePartitioning
{
public:
	std::vector<SurfaceComponent> m_Surfaces;

	std::unique_ptr<BinaryTreePartitioning> m_NodeLeft;

	std::unique_ptr<BinaryTreePartitioning> m_NodeRight;

	BinaryTreePartitioning() = default;
	~BinaryTreePartitioning() = default;

	BinaryTreePartitioning(const std::vector<SurfaceComponent>& surface);
	void Construct(const std::vector<SurfaceComponent>&surface);
	void Clear();

	void Traverse(std::function<void(std::vector<SurfaceComponent>&)>& f);
	Agnode_t* GetCGraphTree(Agraph_t* graph);
};

#endif
