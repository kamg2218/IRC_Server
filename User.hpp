#ifndef USER_HPP
# define USER_HPP

#include "Socket.hpp"
#include <string>

class User
{
	private:
		Socket			_soc;
		std::string		_realname;
		std::string		_nickname;
		std::string		_host;
	public:
		User() {}
		User(std::string const& r, std::string const& n, std::string const& h)
		: _realname(r), _nickname(n), _host(h)
		{}
		~User() { std::cout << "~User()\n"; }
		Socket&		soc() { return _soc; }
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
