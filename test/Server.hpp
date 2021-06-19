#ifndef SERVER_HPP
# define SERVER_HPP

#include "Socket.hpp"
#include <iostream>
#include <exception>
#include <sys/socket.h>

class		Server
{
	private:
		Socket		_soc;
	public:
		Server();
		Server(int port);
		Server(const Server& other);
		Server&		operator=(const Server& other);
		~Server();
		void		makeServer();
		void		handleAccept();
		Socket&		soc();
		//class	BindException;
		//class	ListenException;
		class	BindException : public std::exception
		{
			public:
				virtual const char* what(void) const throw()
				{
					return "Bind Error\n";
				}
		};

		class	ListenException : public std::exception
		{
			public:
				virtual const char* what(void) const throw()
				{
					return "Listen Error\n";
				}
		};
};

#endif
