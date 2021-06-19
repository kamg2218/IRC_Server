#ifndef FT_USER_HPP
# define FT_USER_HPP

#include <string>

class Session;
class User;
#include "Channel.hpp"

typedef std::map<std::string, Channel*>  Channelmap;

class User
{
	private:
		Session			*mysession;
		std::string		sRealname;
		std::string		sNickname;
		std::string		sHost;
		Channelmap		mChannels;
		bool	didNick;
		bool	didUser;
		bool	is_properly_quit;
	public:
		User(Session* ms);
		~User();
		/*
		void	cmdNICK();
		void	cmdUSER();
		void	cmdJOIN();
		*/
		std::string	RealName() const;
		std::string nick() const;
		std::string Host() const;
};
#endif
