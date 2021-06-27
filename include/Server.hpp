#ifndef FT_SERVER_HPP
# define FT_SERVER_HPP

#include <exception>
#include <iostream>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
class Service;
class Session;
//#include "Service.hpp"

class Server
{
	private:
		int		sockfd;
	public:
		class BindException : public std::exception
		{
			public:
				virtual const char *what(void) const throw();
		};
		class ListenException : public std::exception
		{
			public:
				virtual const char *what(void) const throw();
		};
		class SocketException : public std::exception
		{
			public:
				virtual const char *what(void) const throw();
		};
		class AcceptException : public std::exception
		{
			public:
				virtual const char *what(void) const throw();
		};
		Server();
		void	create(unsigned int port);
		Session*	handleAccept(Service *p);
		int		socket() const;
};

#endif
