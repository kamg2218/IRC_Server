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
}

void		Channel::addUser(Session *user)
{
	mUsers[user->user().nick()] = user;
}

void		Channel::removeUser(User *user)
{
	mUsers.erase(mUsers.find(user->nick()));
}

bool		Channel::hasUser(User *user)
{
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

void		Channel::privmsgBroadcast(Session *ss, std::string const& message)
{
	Usermap::iterator it;

	it = mUsers.begin();
	for (; it != mUsers.end() ; ++it)
	{
		if (ss->user().nick() == it->first)
			continue ;
		ss->replyAsUser(it->second, message);
	}
}

std::string		Channel::password() const { return sPassword; }

std::string		Channel::topic() const { return sTopic; }

std::string		Channel::name() const { return sName; }

int				Channel::userCount() const { return mUsers.size(); }

bool			Channel::hasPass() const
{
	return (false);
}

void	Channel::cmdNick(std::string const& name, std::string const& nick)
{
	Usermap::iterator	it;

	it = mUsers.find(name);
	if (it != mUsers.end())
	{
		mUsers.insert(std::pair<std::string, Session*>(nick, it->second));
		mUsers.erase(it);
	}
	it = mOperators.find(name);
	if (it != mOperators.end())
	{
		mOperators.insert(std::pair<std::string, Session*>(nick, it->second));
		mOperators.erase(it);
	}
}

void	Channel::cmdTopic(std::string const& topic, std::string const& msg)
{
	Usermap::iterator	it;

	setTopic(topic);
	for (it = mUsers.begin(); it != mUsers.end(); it++)
		broadcast(it->second, msg);
}

void	Channel::cmdJoin(Session *ss)
{
	std::string	str;
	Usermap::iterator	it;

	if (topic() == "")
		ss->rep331(name());
	else
		ss->rep332(name(), topic());
	it = mUsers.begin();
	if (it != mUsers.end())
	{
		if (mOperators.find(it->first) != mOperators.end())
			str = "@" + it->first;
		else
			str = it->first;
		it++;
	}
	for (; it != mUsers.end(); it++)
	{
		str += " ";
		if (mOperators.find(it->first) != mOperators.end())
			str += "@";
		str += it->first;
	}
	if (str != "")
		ss->rep353(name(), str);
	ss->rep366(name());
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

std::vector<std::string> Channel::channelVector()
{
	Usermap::iterator it;
	std::vector<std::string> res;
	std::string servername = "ft_irc";

	for (it = mUsers.begin(); it != mUsers.end(); ++it)
	{
		res.push_back("#" + name() + " " + it->second->user().user() + " " + it->second->user().host() + " " + servername + " " + it->second->user().nick() + " H :0 " + it->second->user().name());
	}
	return (res);
}

std::vector<std::string> Channel::channeloperVector()
{
	Usermap::iterator it;
	std::vector<std::string> res;
	std::string servername = "ft_irc";

	for (it = mOperators.begin(); it != mOperators.end(); ++it)
	{
		res.push_back("#" + name() + " " + it->second->user().user() + " " + it->second->user().host() + " " + servername + " " + it->second->user().nick() + " H :0 " + it->second->user().name());

	}
	return (res);
}
