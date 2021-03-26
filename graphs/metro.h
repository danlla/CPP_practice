#pragma once
#include <fstream>
#include <string>
#include <exception>

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

struct route
{
	unsigned short index_src;
	unsigned short index_dst;
	link_type lt;
	unsigned short time_transit;
};

struct station
{
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

std::vector<station> load_stations_from_file(char* path_stations)
{
	std::vector<station> stations;
	std::ifstream f(std::string(path_stations), std::ios_base::in);
	if (!f)
		throw std::ios_base::failure(std::string("File didn't open"));
	station tmp;
	while (f >> tmp.index)
	{
		//f >> tmp.index;
		std::getline(f, std::string(), '\t');
		std::getline(f, tmp.line_name, '\t');
		std::getline(f, tmp.station_name, '\t');
		f >> tmp.st;
		f >> tmp.delay;
		f >> tmp.latitude;
		f >> tmp.longitude;
		stations.push_back(tmp);
	}
	return stations;
}

std::vector<route> load_routes_from_file(char* path_routes)
{
	std::vector<route> routes;
	std::ifstream f(std::string(path_routes), std::ios_base::in);
	if (!f)
		throw std::ios_base::failure(std::string("File didn't open"));
	route tmp;
	while (f >> tmp.index_dst)
	{
		//f >> tmp.index_dst;
		f >> tmp.index_src;
		f >> tmp.lt;
		f >> tmp.time_transit;
		routes.push_back(tmp);
	}
	return routes;
}