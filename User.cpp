#include "include/User.hpp"

User::User(std::string const& r, std::string const& n, std::string const& h)
	: sRealname(r), sNickname(n), sHost(h), didNick(false), didUser(false)
{}

std::string		User::RealName() const
{
	return (sRealname);
}

std::string		User::nick() const
{
	return (sNickname);
}

std::string		User::Host() const
{
	return (sHost);
}
