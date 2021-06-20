#ifndef USER_HPP
# define USER_HPP

#include <string>

class User
{
	private:
		std::string		_realname;
		std::string		_nickname;
		std::string		_host;
		//Channelmap	mChannels;
		//bool didNick;
		//bool didUser;
	public:
		User(std::string const& r, std::string const& n, std::string const& h)
		: _realname(r), _nickname(n), _host(h)//, didNick(false), didUser(false)
		{}
		std::string	name() const
		{
			return (_realname);
		}
		std::string nick() const
		{
			return (_nickname);
		}
		std::string host() const
		{
			return (_host);
		}
};
#endif
