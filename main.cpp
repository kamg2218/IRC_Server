#include "include/Frame.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
/*
typedef struct base
{
    std::string host;
    int server_port;
    std::string password_network;
    int port;
    std::string password;
  
} base;
*/

int ft_atoi(char *str)
{
	int n = 0;
	while (*str >= '0' && *str <= '9')
	{
		n = n * 10 + (*str++ - '0');
	}
	return n;
}

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
	
	if (argc > 4 || argc < 3)
	{
		std::cout << "Wrong Input" << std::endl;
		return (1);
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
		a.server_port = ft_stoi(ch);
		a.password_network = v[2];

		a.port = ft_stoi(argv[2]);
		a.password = argv[3];
	}
	else
	{
		a.port = ft_stoi(argv[1]);
		a.password = argv[2];
	}
	a.port = 80;
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
