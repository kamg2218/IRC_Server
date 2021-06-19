#ifndef FT_SESSION_HPP
# define FT_SESSION_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <map>
#include <iostream>
#include "Executor.hpp"
class Session;
class User;
class Service;
#include "User.hpp"
#include "Service.hpp"

class Session
{
	private:
		User	user;
		int		fd;
		//std::string buffer;
		Executor		request;
		//std::string		buffer;
		//std::string	msg;
		Service*	service;

	public:
		typedef Session*	pointer;
		Session(int csfd, Service* svc);
		Session(Service* svc);
		static pointer	create(Service* svc, int);
		int		socket() const;
		bool	handleRead(std::map<int, Session*> & ms);
		void	reply(std::string const& str);
};

#endif
