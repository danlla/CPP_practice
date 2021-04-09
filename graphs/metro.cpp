#include "graph.h"
#include "metro.h"
#include <iostream>
#include <algorithm>
#include <memory>

#include <Windows.h>


std::vector<std::shared_ptr<station>> bellman_ford(graph<std::shared_ptr<station>, std::shared_ptr<route>>& g, std::shared_ptr<station> src, std::shared_ptr<station> dst, std::vector<std::shared_ptr<station>>& stations, unsigned short& way)
{
	std::unordered_map<unsigned short, unsigned short> d;
	std::unordered_map<unsigned short, unsigned short> p;

	const auto max = route::max_time();
	for (const auto& v : g)
	{
		if (v != src)
			d[v->index] = max;
		else
			d[v->index] = 0;
	}

	const auto n = g.size();
	for (size_t i = 0; i < n; ++i)
	{
		for (auto j = g.begin_link(); j != g.end_link(); ++j)
		{
			if(d[j->src->index]<max)
				if (d[j->dst->index] > d[j->src->index] + j->edge->time_transit)
				{
					d[j->dst->index] = d[j->src->index] + j->edge->time_transit;
					p[j->dst->index] = j->src->index;
				}
		}
	}

	std::vector<std::shared_ptr<station>> result;
	result.push_back(dst);
	auto tmp = p[dst->index];
	result.push_back(stations[tmp]);
	while (tmp != src->index)
	{
		tmp = p[tmp];
		result.push_back(stations[tmp]);
	}

	std::reverse(result.begin(), result.end());

	way = d[dst->index];

	return result;
}

struct dijkstra_comparator
{
	std::unordered_map<unsigned short, unsigned short>& d;
	const std::unordered_set<unsigned short>& used;

	dijkstra_comparator(std::unordered_map<unsigned short, unsigned short>& d, const std::unordered_set<unsigned short>& used)
		: d(d), used(used) { }

	bool operator()(std::shared_ptr<station>& src, std::shared_ptr<station>& dst)
	{     // src < dst

		auto end = used.end();
		auto src_used = used.find(src->index) != end;
		auto dst_used = used.find(dst->index) != end;
		if (src_used && dst_used)
		{
			return d[src->index] < d[dst->index];
		}
		else if (src_used)
		{
			return false;
		}
		else if (dst_used)
		{
			return true;
		}
		else
		{
			return d[src->index] < d[dst->index];
		}
	}
};


std::vector<std::shared_ptr<station>> dijkstra(graph<std::shared_ptr<station>, std::shared_ptr<route>>& g, std::shared_ptr<station> src, std::shared_ptr<station> dst,std::vector<std::shared_ptr<station>>& stations, unsigned short& way)
{
	//std::unordered_map<std::shared_ptr<station>, std::shared_ptr<route>> d;
	std::unordered_map<unsigned short, unsigned short> d;
	std::unordered_set<unsigned short> used;
	std::unordered_map<unsigned short, unsigned short> p;

	const auto max = route::max_time();
	for (const auto& v : g)
	{
		if (v != src)
			d[v->index] = max;
		else
			d[v->index] = 0;
	}

	dijkstra_comparator comparator(d, used);
	const auto n = g.size();
	for (size_t i = 0; i < n; ++i)
	{
		auto v = *std::min_element(g.begin(), g.end(), comparator);
		if (d[v->index] == max)
		{
			break;
		}

		used.insert(v->index);

		auto begin = g.begin_adjacent_link(v);
		auto end = g.end_adjacent_link(v);
		while (begin != end)
		{
			auto link = *begin;
			auto q = link.dst;
			auto w = link.edge;
			if (d[v->index] + w->time_transit < d[q->index])
			{
				d[q->index] = d[v->index] + w->time_transit;
				p[q->index] = v->index;
			}
			++begin;
		}
	}

	std::vector<std::shared_ptr<station>> result;
	result.push_back(dst);
	auto tmp = p[dst->index];
	result.push_back(stations[tmp]);
	while (tmp != src->index)
	{
		tmp = p[tmp];
		result.push_back(stations[tmp]);
	}

	std::reverse(result.begin(), result.end());

	way = d[dst->index];

	return result;
}

bool compare_routes_in_src(std::shared_ptr<route>& r1, std::shared_ptr<route>& r2)
{
	return r1->index_src < r2->index_src;
}

void main(void)
{
	system("chcp 65001");
	auto stations = load_stations_from_file("C:\\Users\\USER\\Desktop\\Учеба\\С С++\\CPP_practice\\graphs\\moscow-subway-stations.txt");
	auto routes = load_routes_from_file("C:\\Users\\USER\\Desktop\\Учеба\\С С++\\CPP_practice\\graphs\\moscow-subway-routes.txt");

	graph<std::shared_ptr<station>, std::shared_ptr<route>> metro;

	for (auto& _station : stations)
	{
		metro.add_vertex(_station);
	}

	for (auto& _route : routes)
	{
		metro.add_edge(stations[_route->index_src], stations[_route->index_dst],_route);
	}

	

	std::cout << u8"Введите индекс станции откуда: ";
	unsigned short in_src = 0;
	std::cin >> in_src;

	while (in_src > 193)
	{
		std::cout << u8"Введите индекс станции откуда: ";
		std::cin >> in_src;
	}

	std::cout << u8"Введите индекс станции куда: ";
	unsigned short in_dst = 0;
	std::cin >> in_dst;

	while (in_dst > 193)
	{
		std::cout << u8"Введите индекс станции куда: ";
		std::cin >> in_dst;
	}

	unsigned short way = 0;
	auto res = dijkstra(metro, stations[in_src], stations[in_dst], stations, way);

	std::cout<<u8"Путь займет: " << way/60 <<u8" минут" << std::endl;

	for (auto i = res.begin(); i!=res.end();++i)
	{
		std::cout << i->get()->station_name << "(" <<i->get()->line_name<<")"<< "->";
		
		if(i+1!=res.end()&& i->get()->line_name != (i+1)->get()->line_name)
			std::cout<< u8"Пересадка";
		std::cout << std::endl;
	}
	std::cout << std::endl << std::endl;

	res = bellman_ford(metro, stations[in_src], stations[in_dst], stations, way);

	std::cout << u8"Путь займет: " << way / 60 << u8" минут" << std::endl;

	for (auto i = res.begin(); i != res.end(); ++i)
	{
		std::cout << i->get()->station_name << "(" << i->get()->line_name << ")" << "->";

		if (i + 1 != res.end() && i->get()->line_name != (i + 1)->get()->line_name)
			std::cout << u8"Пересадка";
		std::cout << std::endl;
	}
}