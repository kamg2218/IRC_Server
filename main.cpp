#include "include/Frame.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cstring>

std::vector<std::string> split(std::string str)
{
	std::vector<std::string>	v;
	std::string::size_type		pos;

	while ((pos = str.find(":")) != std::string::npos)
	{
		v.push_back(str.substr(0, pos));
		str.erase(0, str.find(":") + 1);
	}
	return v;
}

int main(int argc, char *argv[])
{
	base a;
	
	if (argc != 3 && argc != 4)
		return (-1);
	if (argc == 4)
	{
		std::vector<std::string> v;

		v = split(argv[1]);
		a.host = v[0];
		a.server_port = std::strtod(v[1].c_str(), 0);
		a.password_network = v[2];

		a.port = std::strtod(argv[2], 0);
		a.password = argv[3];
	}
	else
	{
		a.port = std::strtod(argv[1], 0);
		a.password = argv[2];
	}
	try
	{
		Frame *mainframe = Frame::instance();
		mainframe->start(a);
	}
	catch(std::exception const& err)
	{
		std::cout << err.what() << "\n";
	}
}
