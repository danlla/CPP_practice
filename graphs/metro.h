#pragma once
#include <fstream>
#include <string>
#include <exception>
#include <memory>

enum class link_type : bool
{
	DRIVE = 0,
	CHANGE
};

enum class station_type : unsigned short
{
	PASS = 0,
	CROSS,
	DEADEND
};

std::ifstream& operator>>(std::ifstream& f, station_type& st)
{
	std::string tmp;
	f >> tmp;
	if (tmp == "pass")
		st = station_type::PASS;
	if (tmp == "cross")
		st = station_type::CROSS;
	if (tmp == "deadend")
		st = station_type::DEADEND;
	return f;
}

std::ostream& operator<<(std::ostream& os, station_type& st)
{
	if (st == station_type::PASS)
		os << "pass";
	if (st == station_type::CROSS)
		os << "cross";
	if (st == station_type::DEADEND)
		os << "deadend";
	return os;
}

std::ifstream& operator>>(std::ifstream& f, link_type& st)
{
	std::string tmp;
	f >> tmp;
	if (tmp == "drive")
		st = link_type::DRIVE;
	if (tmp == "change")
		st = link_type::CHANGE;
	return f;
}

std::ostream& operator<<(std::ostream& os, link_type& st)
{
	if (st == link_type::DRIVE)
		os << "drive";
	if (st == link_type::CHANGE)
		os << "change";
	return os;
}

//struct route
//{
//	link_type lt;
//	unsigned short time_transit;
//};

class route //public route
{
public:
	unsigned short index_src;
	unsigned short index_dst;
	link_type lt;
	unsigned short time_transit;



	const static unsigned short TIME_MAX = 30000;
	static unsigned short max_time()
	{
		return TIME_MAX;
	}

	bool operator<(route& obj)
	{
		return time_transit < obj.time_transit;
	}

	bool operator==(const unsigned short obj) const
	{
		return time_transit == obj;
	}

};

class station
{
public:
	unsigned short index;
	std::string line_name;
	std::string station_name;
	station_type st;
	unsigned short delay;
	double latitude;
	double longitude;
};

std::ostream& operator<<(std::ostream& os, station& st)
{
	os << st.index << ' ' << st.line_name << ' ' << st.station_name << ' ' << st.st << ' ' << st.delay << ' ' << st.latitude << ' ' << st.longitude;
	return os;
}

std::vector<std::shared_ptr<station>> load_stations_from_file(char* path_stations)
{
	std::vector<std::shared_ptr<station>> stations;
	std::ifstream f(std::string(path_stations), std::ios_base::in);
	if (!f)
		throw std::ios_base::failure(std::string("File didn't open"));
	unsigned short index;
	while (f >> index)
	{
		auto st = std::make_shared<station>();
		st->index = index;
		std::getline(f, std::string(), '\t');
		std::getline(f, st->line_name, '\t');
		std::getline(f, st->station_name, '\t');
		f >> st->st;
		f >> st->delay;
		f >> st->latitude;
		f >> st->longitude;
		stations.push_back(st);
	}
	return stations;
}

std::vector<std::shared_ptr<route>> load_routes_from_file(char* path_routes)
{
	std::vector<std::shared_ptr<route>> routes;
	std::ifstream f(std::string(path_routes), std::ios_base::in);
	if (!f)
		throw std::ios_base::failure(std::string("File didn't open"));
	unsigned short index_dst;
	while (f >> index_dst)
	{
		auto rt = std::make_shared<route>();
		rt->index_dst = index_dst;
		f >> rt->index_src;
		f >> rt->lt;
		f >> rt->time_transit;
		routes.push_back(rt);
	}
	return routes;
}