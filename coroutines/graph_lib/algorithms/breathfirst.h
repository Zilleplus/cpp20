#include<graph.h>
#include<queue>

namespace glib {
	generator<int> breathfirst_before(const Graph auto& g, int begin_node)
	{
		std::queue<int> to_visit;
		to_visit.push(begin_node);
		while (!to_visit.empty())
		{
			auto node = to_visit.front();
			to_visit.pop();
			co_yield node;

			for (auto child : g.out_edges(node))
			{
				to_visit.push(child);
			}
		}
	}
}