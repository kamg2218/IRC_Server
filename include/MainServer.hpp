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
		std::map<int, Session*>	mSessions;
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
		void		handleRead(std::map<int, Session*>::iterator temp);
		Session*	handleAccept(Service *p);
		void		handleDecline(std::map<int, Session*>::iterator& pos);
		int		socket() const;
		std::map<int, Session*>&		users() { return mSessions; }
		const std::map<int, Session*>&	users() const { return mSessions; }
};

#endif