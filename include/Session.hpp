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

class Session
{
	private:
		Socket		_soc;
		User		_user;
		Executor	request;
		std::string	rstream;

	public:
		typedef Session*	pointer;
		Session();
		Session(int sd);
		static pointer	create();
		int		socket() const;
		Socket&	soc();
		User&	user();
		bool	handleRead(std::map<int, Session*> & ms, int fd);
		void	replyAsServer(std::string const& str);
		void	replyAsUser(Session *target, std::string const& str);
		bool	IsReady() const;
		void	StreamAppend(char *str, int r);
		void	Err_401(std::string const& nick);
		void	Err_402(std::string const& name);
		void	Err_403(std::string const& chname);
		void	Err_404(std::string const& chname);
		void	Err_406(std::string const& nick);
		void	Err_407(std::string const& nick);
		void	Err_411(std::string const& cmd);
		void	Err_412(void);
		void	Err_413(std::string const& mask);
		void	Err_414(std::string const& mask);
		void	Err_431(void);
		void	Err_432(std::string const& nick);
		void	Err_433(std::string const& nick);
		void	Err_442(std::string const& chanme);
		void	Err_443(std::string const& nick, std::string const& chname);
		void	Err_461(std::string const& cmd);
		void	Err_462(void);
		void	Err_464(void);
		void	Err_482(std::string const& chname);
		void	Err_491(void);
};

#endif
