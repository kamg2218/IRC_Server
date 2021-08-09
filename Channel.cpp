#include "include/Channel.hpp"
#include "include/Session.hpp"

Channel::Channel(Session *creator, std::string const& name, std::string const& topic)
	: _sName(name), _sTopic(topic)
{
	_mUsers[creator->user().nick()] = creator;
	_mOperators[creator->user().nick()] = creator;
}

Channel::~Channel()
{
}

void		Channel::addUser(Session *user)
{
	_mUsers[user->user().nick()] = user;
}

void		Channel::addOper(Session *user)
{
	_mOperators[user->user().nick()] = user;
}

void		Channel::removeUser(User *user)
{
	_mUsers.erase(_mUsers.find(user->nick()));
}

bool		Channel::hasUser(User *user)
{
	Usermap::iterator		it;

	it = _mUsers.find(user->nick());
	if (it == _mUsers.end())
		return false;
	return true;
}

void		Channel::broadcast(Session *ss, std::string const& message)
{
	Usermap::iterator		it;

	it = _mUsers.begin();
	for (; it != _mUsers.end() ; ++it)
		ss->replyAsUser(it->second, message);
}

void		Channel::privmsgBroadcast(Session *ss, std::string const& message)
{
	Usermap::iterator		it;

	it = _mUsers.begin();
	for (; it != _mUsers.end() ; ++it)
	{
		if (ss->user().nick() == it->first)
			continue ;
		ss->replyAsUser(it->second, message);
	}
}

std::string		Channel::topic() const
{
	return _sTopic;
}

std::string		Channel::name() const
{
	return _sName;
}

int		Channel::userCount() const 
{
	return _mUsers.size();
}

bool		Channel::hasPass() const
{
	return false;
}

void		Channel::cmdNick(std::string const& name, std::string const& nick)
{
	Usermap::iterator	it;

	it = _mUsers.find(name);
	if (it != _mUsers.end())
	{
		_mUsers.insert(std::pair<std::string, Session*>(nick, it->second));
		_mUsers.erase(it);
	}
	it = _mOperators.find(name);
	if (it != _mOperators.end())
	{
		_mOperators.insert(std::pair<std::string, Session*>(nick, it->second));
		_mOperators.erase(it);
	}
}

void	Channel::cmdTopic(Session *ss, std::string const& topic, std::string const& msg)
{
	Usermap::iterator	it;

	setTopic(topic);
	for (it = _mUsers.begin(); it != _mUsers.end(); it++)
		broadcast(ss, msg);
}

void	Channel::cmdJoin(Session *ss)
{
	std::string		str;
	Usermap::iterator		it;

	if (topic() == "")
		ss->rep331(name());
	else
		ss->rep332(name(), topic());
	for (it = _mUsers.begin(); it != _mUsers.end(); it++)
	{
		if (it != _mUsers.begin())
			str += " ";
		if (_mOperators.find(it->first) != _mOperators.end())
			str += "@";
		str += it->first;
	}
	if (str != "")
		ss->rep353(name(), str);
	ss->rep366(name());
}

bool		Channel::isOperator(std::string const& nick) const
{
	Usermap::const_iterator		res;

	res = _mOperators.find(nick);
	if (res == _mOperators.end())
		return false;
	return true;
}
void		Channel::setTopic(std::string const topic)
{
	_sTopic = topic;
}

std::vector<std::string>		Channel::channelVector()
{
	Usermap::iterator		it;
	std::vector<std::string>		res;
	std::string servername = "ft_irc";

	for (it = _mUsers.begin(); it != _mUsers.end(); ++it)
	{
		if (isOperator(it->second->user().nick()))
			res.push_back("#" + name() + " " + it->second->user().user() + " " + it->second->user().host() + " " + servername + " " + it->second->user().nick() + " H @ :0 " + it->second->user().name());
		else
			res.push_back("#" + name() + " " + it->second->user().user() + " " + it->second->user().host() + " " + servername + " " + it->second->user().nick() + " H :0 " + it->second->user().name());
	}
	return res;
}

std::vector<std::string>		Channel::channeloperVector()
{
	Usermap::iterator		it;
	std::vector<std::string>		res;
	std::string		servername = "ft_irc";

	for (it = _mOperators.begin(); it != _mOperators.end(); ++it)
	{
		res.push_back("#" + name() + " " + it->second->user().user() + " " + it->second->user().host() + " " + servername + " " + it->second->user().nick() + " H @ :0 " + it->second->user().name());

	}
	return res;
}
