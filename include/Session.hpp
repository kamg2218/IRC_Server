#ifndef FT_SESSION_HPP
# define FT_SESSION_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <map>
#include <iostream>
#include "Executor.hpp"
class Session;
class User;
class Service;
#include "User.hpp"
#include "Service.hpp"
#include "Socket.hpp"
#include "Server.hpp"

class Session
{
	private:
		Socket		_soc;
		User*		_user;
		Server*		_server;
		Executor	request;
		std::string	buff;
		//Service*	service;

	public:
		typedef Session*	pointer;
		Session(int csfd, Service* svc);
		Session(Service* svc);
		static pointer	create(Service* svc, int);
		int		socket() const;
		Socket&	soc();
		bool	handleRead(std::map<int, Session*> & ms, int fd);
		void	reply(std::string const& str);
};

#endif
