#ifndef FT_USER_HPP
# define FT_USER_HPP

#include <string>

class User;
#include "Channel.hpp"

typedef std::map<std::string, Channel*>  Channelmap;

class User
{
	private:
		std::string		sRealname;
		std::string		sNickname;
		std::string		sHost;
		Channelmap		mChannels;
		bool	didNick;
		bool	didUser;
	public:
		User(std::string const& r, std::string const& n, std::string const& h);
		std::string	RealName() const;
		std::string nick() const;
		std::string Host() const;
};
#endif
