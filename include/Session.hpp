#ifndef FT_SESSION_HPP
# define FT_SESSION_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <map>
#include <iostream>
#include "Message.hpp"
#include "User.hpp"
//#include "Shared_Ptr.hpp"

class Session
{
	private:
		User	*user;
		int		fd;
		//std::string buffer;
		Message buffer;
		std::string msg;

	public:
		typedef Session* pointer;
		//typedef Shared_Ptr 	pointer;
		Session(int csfd, struct sockaddr_in const& csinfo);
		Session();
		static pointer create();
		int socket() const;
		bool handleRead(std::map<int, Session*> & ms);
		void reply(std::string const& str);
};

#endif
