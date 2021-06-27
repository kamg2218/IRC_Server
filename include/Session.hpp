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
		Socket	_soc;
		Executor		request;
		//Service*	service;

	public:
		typedef Session*	pointer;
		Session(int csfd, Service* svc);
		Session(Service* svc);
		static pointer	create(Service* svc, int);
		int		socket() const;
		Socket&	soc();
		bool	handleRead(std::map<int, Session*> & ms);
		void	reply(std::string const& str);
};

#endif
