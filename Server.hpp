#ifndef FT_SERVER_HPP
# define FT_SERVER_HPP

#include <exception>
#include <iostream>
#include "User.hpp"
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Server
{
	private:
		int sockfd;
	public:
		class BindException : public std::exception
		{
			public:
			virtual const char *what(void) const throw()
			{
				return ("Bind Error");
			}
		};
		class ListenException : public std::exception
		{
			public:
			virtual const char *what(void) const throw()
			{
				return ("Listen Error\n");
			}
		};
		class SocketException : public std::exception
		{
			public:
			virtual const char *what(void) const throw()
			{
				return ("Socket Error\n");
			}
		};
		class AcceptException : public std::exception
		{
			public:
			virtual const char *what(void) const throw()
			{
				return ("Accpet Error");
			}
		};
		Server()
		{
		}
		void create(unsigned int port)
		{
			struct protoent *pe;
			struct sockaddr_in	sin;
			pe = getprotobyname("tcp");
			sockfd = socket(PF_INET, SOCK_STREAM, pe->p_proto);
			if (!sockfd)
				throw (SocketException());
			sin.sin_family = AF_INET;
			sin.sin_addr.s_addr = htonl(INADDR_ANY);
			sin.sin_port = htons(port);
			if ((bind(sockfd, (struct sockaddr *)&sin, sizeof(sin))) == -1)
				throw (BindException());
			if (listen(sockfd, 42) < 0)
				throw (ListenException());
		}
		void startaccept()
		{
			int cs;
			struct sockaddr_in	csin;
			socklen_t	csin_len;

			csin_len = sizeof(csin);
			cs = accept(sockfd, (struct sockaddr*)&csin, &csin_len);
			if (cs < 0)
				throw (AcceptException());
			std::cout << inet_ntoa(csin.sin_addr) << ":" << ntohs(csin.sin_port) << " is connected\n";
		}
		int getSockfd()
		{
			return (sockfd);
		}
};

#endif
