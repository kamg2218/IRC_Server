#ifndef MAINSERVER_HPP
# define MAINSERVER_HPP

#include <exception>
#include <iostream>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>
class Service;
class Session;
#include "Socket.hpp"

class MainServer
{
	private:
		int		_sd;
		Socket	_sock;
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
		MainServer();
		void	create(unsigned int port);
		Session*	handleAccept(Service *p);
		void		handleDecline(std::map<int, Session*>& mSessions, std::map<int, Session*>::iterator& pos);
		//void		handleDecline(Server& srv);
		//void		handleDecline(Channel& chn);
		int		socket() const;
};

#endif
