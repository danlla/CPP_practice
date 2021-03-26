
#include <algorithm>
#include <cstdio>
#include <limits>
#include <stdexcept>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

template<typename TVertex, typename TEdge>
struct link
{
	TVertex src;
	TVertex dst;
	TEdge edge;

	link(const TVertex& src, const TVertex& dst, const TEdge& edge) : src(src), dst(dst), edge(edge) { }
};

template
<
	typename TVertex,
	typename TEdge,
	typename TVertexContainer = std::vector<TVertex>,
	typename TEdgeContainer = std::vector<link<TVertex, TEdge>>
>
class graph
{
	TVertexContainer _vertices;
	std::unordered_map<TVertex, TEdgeContainer> _edges;

public:
	using VertexIterator = typename TVertexContainer::iterator;
	using AdjacentLinkIterator = typename TEdgeContainer::iterator;

	void add_vertex(const TVertex& vertex)
	{
		auto begin = _vertices.begin();
		auto end = _vertices.end();
		if (std::find(begin, end, vertex) != end)
		{
			throw std::logic_error("Vertex is already inserted!");
		}
		_vertices.insert(_vertices.end(), vertex);
	}

	void add_edge(const TVertex& src, const TVertex& dst, const TEdge& edge)
	{
		// TODO Проверить src и dst - вершины графа
		if (std::find(_vertices.begin(), _vertices.end(), src) == _vertices.end() || std::find(_vertices.begin(), _vertices.end(), dst) == _vertices.end())
			throw std::logic_error("Vertices don't exist");
		link<TVertex, TEdge> new_link(src, dst, edge);
		auto& adjacent_edges = _edges[src];
		adjacent_edges.insert(adjacent_edges.end(), new_link);
	}

	size_t size() const
	{
		return _vertices.size();
	}

	VertexIterator begin()
	{
		return _vertices.begin();
	}

	VertexIterator end()
	{
		return _vertices.end();
	}

	AdjacentLinkIterator begin_adjacent_link(const TVertex& src)
	{
		return _edges[src].begin();
	}

	AdjacentLinkIterator end_adjacent_link(const TVertex& src)
	{
		return _edges[src].end();
	}

	class LinkIterator
	{
		typename std::unordered_map<TVertex, TEdgeContainer>::iterator it_map;
		typename std::unordered_map<TVertex, TEdgeContainer>::iterator it_end;
		AdjacentLinkIterator it_edge;

		void find_next()
		{
			while (it_map != it_end && it_map->second.empty())
			{
				++it_map;
			} 
			if(it_map!=it_end)
				it_edge = it_map->second.begin();
		}
	public:
		LinkIterator(typename std::unordered_map<TVertex, TEdgeContainer>::iterator it_map_, typename std::unordered_map<TVertex, TEdgeContainer>::iterator it_end_)
		{
			it_map = it_map_;
			it_end = it_end_;
			if(it_map!=it_end)
				find_next();
		}

		LinkIterator& operator++()
		{
			++it_edge;
			if (it_edge == it_map->second.end())
			{
				++it_map;
				find_next();
			}
			return *this;
		}

		bool operator == (LinkIterator& lhs)
		{
			return !(*this != lhs);
		}

		bool operator != (LinkIterator& lhs)
		{
			if (it_map != lhs.it_map)
				return true;
			if (it_map == it_end && lhs.it_map == lhs.it_end)
				return false;
			return it_edge != lhs.it_edge;
		}

		link<TVertex, TEdge>& operator*()
		{
			return *it_edge;
		}
	};

	LinkIterator begin_link()
	{
		return LinkIterator(_edges.begin(),_edges.end());
	}

	LinkIterator end_link()
	{
		return LinkIterator(_edges.end(), _edges.end());
	}

};

template
<
	typename TVertex,
	typename TEdge
>
void recur_for_dfs(graph<TVertex, TEdge>& g, TVertex ver, std::unordered_set<TVertex>& visited)
{
	if (visited.find(ver) != visited.end())
		return;
	std::cout << ver;
	visited.insert(ver);
	for (auto it = g.begin_adjacent_link(ver); it != g.end_adjacent_link(ver); ++it)
		recur_for_dfs(g,it->dst, visited);
}

template
<
	typename TVertex,
	typename TEdge
>
void dfs_recur_print(graph<TVertex, TEdge>& g, TVertex ver)
{
	std::unordered_set<TVertex> visited;
	recur_for_dfs(g, ver, visited);
}

template
<
	typename TVertex,
	typename TEdge
>
void bfs_print(graph<TVertex, TEdge>& g,TVertex ver)
{
	std::queue<TVertex> q;
	std::unordered_set<TVertex> visited;
	q.push(ver);
	while (q.size())
	{
		auto tmp = q.front();
		visited.insert(tmp);
		std::cout << *tmp << std::endl;
		q.pop();
		for (auto it = g.begin_adjacent_link(tmp); it != g.end_adjacent_link(tmp); ++it)
		{
			if (visited.find(it->dst) == visited.end())
				q.push(it->dst);
		}
	}
}

template
<
	typename TVertex,
	typename TEdge
>
void dfs_print(graph<TVertex, TEdge>& g, TVertex ver)
{
	std::stack<TVertex> q;
	std::unordered_set<TVertex> visited;
	q.push(ver);
	while (q.size())
	{
		auto tmp = q.top();
		visited.insert(tmp);
		std::cout << tmp;
		q.pop();
		for (auto it = g.begin_adjacent_link(tmp); it != g.end_adjacent_link(tmp); ++it)
		{
			if (visited.find(it->dst) == visited.end())
			{
				q.push(it->dst);
				visited.insert(it->dst);
			}
		}
	}
}

struct dijkstra_comparator
{
	std::unordered_map<char, double>& d;
	const std::unordered_set<char>& used;

	dijkstra_comparator(std::unordered_map<char, double>& d, const std::unordered_set<char>& used)
		: d(d), used(used) { }

	bool operator()(char src, char dst)
	{     // src < dst

		auto end = used.end();
		auto src_used = used.find(src) != end;
		auto dst_used = used.find(dst) != end;
		if (src_used && dst_used) {
			return d[src] < d[dst];
		}
		else if (src_used) {
			return false;
		}
		else if (dst_used) {
			return true;
		}
		else {
			return d[src] < d[dst];
		}
	}
};

double dijkstra(graph<char, double>& g, char src, char dst)
{
	// TODO src, dst включены в граф?

	std::unordered_map<char, double> d;
	std::unordered_set<char> used;
	std::unordered_map<char, char> p;

	const auto max =
		std::numeric_limits<double>::has_infinity
		? std::numeric_limits<double>::infinity()
		: std::numeric_limits<double>::max();
	for (const auto& v : g)
	{
		if (v != src)
			d[v] = max;
		else
			d[v] = 0;
	}

	dijkstra_comparator comparator(d, used);
	const auto n = g.size();
	for (size_t i = 0; i < n; ++i)
	{
		// Шаг 1
		auto v = *std::min_element(g.begin(), g.end(), comparator);
		if (d[v] == max)
		{
			break;
		}

		// Шаг 2
		used.insert(v);

		// Шаг 3
		auto begin = g.begin_adjacent_link(v);
		auto end = g.end_adjacent_link(v);
		while (begin != end) 
		{
			auto link = *begin;
			auto q = link.dst;
			auto w = link.edge;
			if (d[v] + w < d[q])
			{
				d[q] = d[v] + w;
				p[q] = v;
			}
			++begin;
		}
	}

	return d[dst];
}
