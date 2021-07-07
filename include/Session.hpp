#ifndef SESSION_HPP
# define SESSION_HPP

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
	public:
		typedef Session*	pointer;
		Session();
		Session(int sd);
		static pointer	create();
		int		socket() const;
		Socket&	soc();
		bool	handleRead(std::map<int, Session*> & ms, int fd);
		void	reply(std::string const& str);
};

#endif
