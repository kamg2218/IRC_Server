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
		void	Err_401();
		void	Err_402();
		void	Err_404();
		void	Err_406();
		void	Err_407();
		void	Err_411();
		void	Err_412();
		void	Err_413();
		void	Err_414();
		void	Err_431();
		void	Err_432();
		void	Err_433();
		void	Err_442();
		void	Err_443();
		void	Err_461(std::string const& str);
		void	Err_462(std::string const& str);
		void	Err_464(std::string const& str);
		void	Err_482(std::string const& str);
		void	Err_491(std::string const& str);
};

#endif
