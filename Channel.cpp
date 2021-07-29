#include "include/Channel.hpp"
#include "include/Session.hpp"

Channel::Channel(Session *creator, std::string const& name, std::string const& topic)
	: sName(name), sTopic(topic)
{
	mUsers[creator->user().nick()] = creator;
	mOperators[creator->user().nick()] = creator;
}

Channel::~Channel()
{
	//write
}

void		Channel::addUser(Session *user)
{
	//write
	mUsers[user->user().nick()] = user;
	
}

void		Channel::removeUser(User *user)
{
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

void		Channel::broadcast(Session *ss, std::string const& message)
{
	Usermap::iterator it;

	it = mUsers.begin();
	for (; it != mUsers.end() ; ++it)
		ss->replyAsUser(it->second, message);
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
			mUsers.insert(std::pair<std::string, Session*>(nick, it->second));
			mUsers.erase(it);
			return ;
		}
	}
}

void	Channel::cmdJoin(Session *ss)
{
	std::string	str;
	Usermap::iterator	it;

	if (topic() == "")
		ss->Rep_331(name());
	else
		ss->Rep_332(name(), topic());
	it = mUsers.begin();
	if (it != mUsers.end())
	{
		str = it->first;
		it++;
	}
	for (; it != mUsers.end(); it++)
		str += " " + it->first;
	if (str != "")
		ss->Rep_353(name(), str);
	ss->Rep_366(name());
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
