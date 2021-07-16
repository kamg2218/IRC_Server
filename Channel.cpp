#include "include/Channel.hpp"
#include "include/Session.hpp"

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

void		Channel::broadcast(Session *ss, std::string const& message)
{
	Usermap::iterator it;

	it = mUsers.begin();
	for (; it != mUsers.end() ; ++it)
		ss->reply(message);
}

std::string		Channel::password() const { return sPassword; }

std::string		Channel::topic() const { return sTopic; }

std::string		Channel::name() const { return sName; }

int				Channel::userCount() const { return mUsers.size(); }

bool			Channel::hasPass() const
{
	//write
	return (false);
}

void	Channel::cmdNick(std::string const& name, std::string const& nick)
{
	for (Usermap::iterator it = mUsers.begin(); it != mUsers.end(); it++)
	{
		if (it->first == name)
		{
			mUsers.insert(std::pair<std::string, User*>(nick, it->second));
			mUsers.erase(it);
			return ;
		}
	}
}


bool			Channel::isOperator(std::string const& nick) const
{
	Usermap::const_iterator	res;

	res = mOperators.find(nick);
	if (res == mOperators.end())
		return (false);
	return (true);
}
void	Channel::setTopic(std::string const topic)
{
	sTopic = topic;

}
