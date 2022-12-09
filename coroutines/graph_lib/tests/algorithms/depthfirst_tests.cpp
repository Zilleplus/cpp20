#include<catch2/catch_test_macros.hpp>
#include<algorithms/depthfirst.h>

glib::graph TestGraphDepthFirst()
{
	glib::graph g;

	std::vector<int> nodes;
	for (int i = 0; i < 10; ++i)
	{
		nodes.push_back(g.add_node());
	}

	g.add_edge(nodes[0], nodes[1]);
	g.add_edge(nodes[0], nodes[2]);

	g.add_edge(nodes[1], nodes[3]);
	g.add_edge(nodes[1], nodes[4]);

	g.add_edge(nodes[2], nodes[5]);
	g.add_edge(nodes[2], nodes[6]);

	g.add_edge(nodes[3], nodes[7]);
	g.add_edge(nodes[3], nodes[8]);

	g.add_edge(nodes[4], nodes[9]);

	return g;
}

TEST_CASE("Given_Graph_Check_Depth_First_Before")
{
	auto g = TestGraphDepthFirst();
	std::vector<int> expected_ordered_nodes = { 0, 2, 6, 5, 1, 4, 9, 3, 8, 7 };

	std::vector<int> result;
	for (auto n : glib::depthfirst_before(g, 0))
	{
		result.push_back(n);
	}

	REQUIRE(result == expected_ordered_nodes);
}
