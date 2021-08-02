#include "include/Frame.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cstring>

std::vector<std::string> split(std::string str, std::string divid)
{
	std::vector<std::string> v;
	char* c = strtok((char*)str.c_str(), divid.c_str());
	while (c) {
		v.push_back(c);
		c = strtok(NULL, divid.c_str());
	}
	return v;
}

int main(int argc, char *argv[])
{
	base a;
	
	if (argc != 3 || argc != 4)
	{
		std::cout << "Wrong Input" << std::endl;
		return (-1);
	}
	/*
	* argv[1] parse
	*/
	if (argc == 4)
	{
		std::vector<std::string> v = split(argv[1], ":");
		a.host = v[0];
		char ch[100];
		strcpy(ch,v[1].c_str());
		a.server_port = std::strtod(ch, 0);
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
