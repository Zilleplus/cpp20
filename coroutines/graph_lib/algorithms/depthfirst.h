#include<graph.h>
#include<stack>

namespace glib {
	generator<int> depthfirst_before(const Graph auto& g, int begin_node)
	{
		std::stack<int> to_visit;
		to_visit.push(begin_node);
		while (!to_visit.empty())
		{
			auto node = to_visit.top();
			to_visit.pop();
			co_yield node;

			for (auto child : g.out_edges(node))
			{
				to_visit.push(child);
			}
		}
	}
}
