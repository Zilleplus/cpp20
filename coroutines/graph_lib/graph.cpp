#include "graph.h"
#include <coroutine>

namespace glib {
	generator<int>graph::out_edges(int node) const
	{
		auto from_iter = edges.find(node);
		if (from_iter == edges.end())
		{
			co_return;
		}

		for (auto n : from_iter->second)
		{
			co_yield n;
		}
	}

	generator<int>graph::all_nodes() const
	{
		for (auto n : this->nodes)
		{
			co_yield n;
		}
	}

	int graph::add_node() {
		const auto new_node_id = next_node_id;
		next_node_id = next_node_id + 1;
		nodes.push_back(new_node_id);

		return new_node_id;
	}

	void graph::add_edge(int from, int to)
	{
		if (edges.contains(from))
		{
			edges[from].push_back(to);
		}
		else {
			std::vector<int> outs = { to };
			edges[from] = outs;
		}
	}
}