#ifndef FT_CHANNEL_HPP
#define FT_CHANNEL_HPP

#include <string>
#include <map>

class Channel;
#include "User.hpp"

typedef std::map<std::string, User*> Usermap;

class Channel
{
	private:
		const std::string	sName;
		std::string 		sTopic;
		std::string			sPassword;
		bool				bPassword;
		Usermap 			mUsers;
	public:
		std::string name() const;
};
#endif
