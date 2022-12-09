#pragma once
#include<generator.h>
#include<vector>
#include<map>

namespace glib {
	template<typename T>
	concept Graph = requires(T a)
	{
		{ std::declval<T>().out_edges(std::declval<int>())};
		{ std::declval<T>().all_nodes()};
	};

	class graph {
		int next_node_id = 0;
		std::vector<int> nodes;
		std::map<int, std::vector<int>> edges;
	public:
		generator<int> out_edges(int node) const;
		generator<int> all_nodes() const;

		int add_node();
		void add_edge(int from, int to);
	};
}