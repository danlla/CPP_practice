#include "graphs.h"
#include <memory>
#include <Windows.h>



void main(void)
{
	system("chcp 65001");
	std::vector<station> stations = load_stations_from_file("C:\\Users\\USER\\Desktop\\׃קובא\\ׁ ׁ++\\CPP_practice\\graphs\\moscow-subway-stations.txt");
	std::vector<route> routes = load_routes_from_file("C:\\Users\\USER\\Desktop\\׃קובא\\ׁ ׁ++\\CPP_practice\\graphs\\moscow-subway-routes.txt");

	graph<std::shared_ptr<station>, std::shared_ptr<route>> metro;

	for (auto& _station : stations)
	{
		metro.add_vertex(std::shared_ptr<station>(&_station));
	}

	for (auto& _route : routes)
	{
		metro.add_edge(std::shared_ptr<station>(&stations[_route.index_src]), std::shared_ptr<station>(&stations[_route.index_dst]),std::shared_ptr<route>(&_route));
	}

	bfs_print(metro, std::shared_ptr<station>(&stations[0]));
}