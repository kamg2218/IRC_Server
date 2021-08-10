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
    int		_port;
    int		_serverPort;
    std::string		_host;
    std::string		_password;
    std::string		_passwordNetwork;
  
} base;

class MainServer
{
	private:
		int				_sd;
		Socket			_sock;
		std::string		_pass;
		std::string		_name;
		std::map<int, Session*>		_mSessions;
		MainServer(MainServer const& other);
		MainServer&		operator=(MainServer const& other);
	public:
		MainServer(void);
		~MainServer(void);
		void		handleAccept(void);
		void		create(base const& bs);
		void		setPass(std::string const& pass);
		void		handleRead(std::map<int, Session*>::iterator temp);
		void		handleDecline(std::map<int, Session*>::iterator const& pos);
		int			socket(void) const;
		bool		checkPass(std::string const& pass);
		std::string	msgHeader(void);
		std::string	name(void) const;
		std::map<int, Session*>&		users(void);
		const std::map<int, Session*>&	users(void) const;
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
};

#endif
