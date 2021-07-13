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
	mUsers[user->nick()] = user;
	
}

void		Channel::removeUser(User *user)
{
	//write
	mUsers.erase(mUsers.find(user->nick()));
}

bool		Channel::hasUser(User *user)
{
	//write
	Usermap::iterator it;

	it = mUsers.find(user->nick());
	if (it == mUsers.end())
		return (false);
	return (true);
}

bool		Channel::isOperator(User *user)
{
	//write	
	Usermap::iterator it;

	it = mOperators.find(user->nick());
	if (it == mOperators.end())
		return (false);

	return (true);
}

void		Channel::broadcast(std::string const& message)
{
	//write
	Usermap::iterator it;

	it = mUsers.begin();
	for (; it != mUsers.end() ; ++it)
	{
		//(*it).second->session->sendAsUser(message);
	}
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

bool			Channel::isOperator(std::string const& nick) const
{
	Usermap::const_iterator	res;

	res = mOperators.find(nick);
	if (res == mOperators.end())
		return (false);
	return (true);
}
