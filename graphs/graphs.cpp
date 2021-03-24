#include "graphs.h"


int main(void)
{
	graph<char, double> g;
	g.add_vertex('A');
	g.add_vertex('B');
	g.add_vertex('C');
	g.add_vertex('D');
	g.add_vertex('E');
	g.add_vertex('F');
	g.add_edge('A', 'B', 1);
	g.add_edge('B', 'C', 1);
	g.add_edge('C', 'D', 1);
	g.add_edge('D', 'E', 1);
	g.add_edge('A', 'E', 9);
	g.add_edge('A', 'F', 1);
	g.add_edge('F', 'E', 9);

	for (auto& v : g)
	{
		printf("%c\n", v);
		{
			auto begin = g.begin_adjacent_link(v);
			auto end = g.end_adjacent_link(v);
			while (begin != end) {
				printf("\t %c -> %c (%f)\n", begin->src, begin->dst, begin->edge);
				++begin;
			}
		}
	}

	printf("%f", dijkstra(g, 'A', 'E'));

	std::cout << "\n\n\n\n\n";
	for (auto it = g.begin_link(); it != g.end_link(); ++it)
	{
		std::cout <<" src "<<(*it).src<<" dst "<<(*it).dst<<" edge "<< (*it).edge << std::endl;
	}

	//bfs_print(g,'A');
}
