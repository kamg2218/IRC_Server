#ifndef SERVER_HPP
# define SERVER_HPP

#include <exception>
#include <iostream>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Socket.hpp"

class Server
{
	private:
		Socket		_soc;
		std::string	_name;
	public:
		Server();
};

#endif
