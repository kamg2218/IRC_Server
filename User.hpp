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
		bool didNick;
		bool didUser;
	public:
		std::string	RealName() const
		{
			return (sRealname);
		}
		std::string nick() const
		{
			return (sNickname);
		}
		std::string Host() const
		{
			return (sHost);
		}

};
#endif
