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

typedef struct base
{
    std::string host;
    int server_port;
    std::string password_network;
    int port;
    std::string password;
  
} base;

class MainServer
{
	private:
		int		_sd;
		Socket	_sock;
		std::string	_pass;
		std::string	_name;
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
		void	create(base const& bs);
		void	handleRead(std::map<int, Session*>::iterator temp);
		void	handleAccept(Service *p);
		void	handleDecline(std::map<int, Session*>::iterator const& pos);
		int		socket() const;
		std::string	name() const;
		std::map<int, Session*>&		users();
		const std::map<int, Session*>&	users() const;
		bool	checkPass(std::string const& pass);
		void	setPass(std::string const& pass);
		std::string		msgHeader();
};

#endif
