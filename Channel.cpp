#include "include/Channel.hpp"

Channel::Channel(User *creator, std::string const& name, std::string const& topic)
	: sName(name), sTopic(topic)
{
	mUsers[creator->nick()] = creator;
	mOperators[creator->nick()] = creator;
}

Channel::~Channel()
{
	//write
}

void		Channel::addUser(User *user)
{
	//write
}

void		Channel::removeUser(User *user)
{
	//write
}

bool		Channel::hasUser(User *user)
{
	//write
	return (true);
}

bool		Channel::isOperator(User *user)
{
	//write
	return (true);
}

void		Channel::broadcast(std::string const& message)
{
	//write
}

std::string	Channel::password() const
{
	return (sPassword);
}

std::string Channel::topic() const
{
	return (sTopic);
}

std::string		Channel::name() const
{
	return (sName);
}

int				Channel::userCount() const
{
	return (mUsers.size());
}

bool			Channel::hasPass() const
{
	//write
	return (false);
}
