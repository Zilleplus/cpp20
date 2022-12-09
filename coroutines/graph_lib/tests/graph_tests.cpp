#include<catch2/catch_test_macros.hpp>
#include<graph.h>
#include<tuple>


std::tuple<glib::graph, int, int, int> ExampleGraph() {
	glib::graph g;
	auto n1 = g.add_node();
	auto n2 = g.add_node();
	auto n3 = g.add_node();

	return { g, n1, n2, n3 };
}

TEST_CASE("Give_Graph_Check_Nodes")
{
	auto [g, n1, n2, n3] = ExampleGraph();

	auto nodes = g.all_nodes();

	nodes.resume();
	REQUIRE(nodes.value() == n1);
	nodes.resume();
	REQUIRE(nodes.value() == n2);
	nodes.resume();
	REQUIRE(nodes.value() == n3);

	nodes.resume();
	REQUIRE(nodes.isDone());
}

TEST_CASE("Given_Graph_Check_Edges")
{

}
