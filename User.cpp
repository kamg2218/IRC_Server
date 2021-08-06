#include "include/User.hpp"
#include "include/Session.hpp"
#include "include/Frame.hpp"

User::User(void)
	: _didUser(false), _didNick(false), _isProperlyQuit(false), _manager(false), _password(true)
{
}

User::User(User const& other)
	: _didUser(false), _didNick(false), _isProperlyQuit(false), _manager(false), _password(true)
{
	*this = other;
}

User&	User::operator=(User const& other)
{
	if (this == &other)
		return *this;
	this->_sRealname = other._sRealname;
	this->_sNickname = other._sNickname;
	this->_sHostname = other._sHostname;
	this->_sUsername = other._sUsername;
	this->_didNick = other._didNick;
	this->_didUser = other._didUser;
	this->_manager = other._manager;
	this->_password = other._password;
	this->_isProperlyQuit = other._isProperlyQuit;
	this->_pastNick = other._pastNick;
	return *this;
}

User::~User(void)
{
	if (isConnected() && !_isProperlyQuit)
	{
		Frame	*frame;
		ChannelMap::iterator it;

		frame = Frame::instance();
		it = _mChannels.begin();
		for (; it != _mChannels.end() ; ++it)
		{
			it->second->broadcast(frame->findUser(_sNickname), "PART #" + it->second->name());
			it->second->removeUser(this);
		}
		_mChannels.clear();
		frame->broadcastAll(frame->findUser(_sNickname), "QUIT :User lost connection");
		frame->removeUser(_sNickname);
	}
}

User*			User::create(void)
{
	return new User();
}

void			User::setName(std::string const& s)
{
	_sRealname = s;
}
void			User::setNick(std::string const& s)
{
	_sNickname = s;
}
void			User::setHost(std::string const& s)
{
	_sHostname = s;
}
void			User::setUser(std::string const& s)
{
	_sUsername = s;
}
void			User::setPass(bool pass)
{
	_password = pass;
}
std::string		User::name(void) const
{
	return _sRealname;
}
std::string		User::nick(void) const
{
	return _sNickname;
}
std::string		User::host(void) const
{
	return _sHostname;
}
std::string		User::user(void) const
{
	return _sUsername;
}
bool			User::pass(void) const
{
	return _password;
}
const ChannelMap&		User::channel(void) const
{
	return _mChannels;
}

std::string		User::msgHeader(void) const
{
	return std::string(":" + _sNickname + "!" + _sUsername + "@" + _sHostname+ " ");
}

bool	User::checkNick(void) const
{
	return _didNick;
}
bool	User::checkUser(void) const
{
	return _didUser;
}
bool	User::isConnected(void) const
{
	if (checkNick() && checkUser())
		return true;
	return false;
}
bool	User::checkManager(void) const
{
	return _manager;
}

bool	User::addNick(std::vector<std::string> const& sets)
{
	if (_didNick)
		return false;	//alreadyRegistered
	_sNickname = sets[1];
	_didNick = true;
	return true;
}

void	User::cmdNick(std::vector<std::string> const& sets)
{
	_pastNick.insert(_pastNick.end(), _sNickname);
	for (ChannelMap::iterator it = _mChannels.begin(); it != _mChannels.end(); it++)
		it->second->cmdNick(_sNickname, sets[1]);
	_sNickname = sets[1];
	return ;
}

bool		User::cmdUser(std::vector<std::string> const& sets)
{
	std::string	str;

	if (_didUser == true)
		return false;
	_didUser = true;
	setUser(sets[1]);
	str = sets[4];
	if (sets[4][0] == ':')
		str = sets[4].substr(1);
	for (int i = 5; i < sets.size(); i++)
		str += " " + sets[i];
	setName(str);
	return true;
}

void	User::cmdJoin(Channel* ch)
{
	_mChannels.insert(std::pair<std::string, Channel*>(ch->name(), ch));
}

void	User::optionJoin(Session *ss, std::vector<std::string> const& sets, std::string const& msg)
{
	for (ChannelMap::iterator it = _mChannels.begin(); it != _mChannels.end(); it++)
	{
		it->second->removeUser(this);
		it->second->broadcast(ss, msg);
		if (it->second->userCount() == 0)
		{
			delete it->second;
			Frame::instance()->removeChannel(it->first);
		}
		_mChannels.erase(it);
	}
}

std::string	User::makeLower(std::string const& str)
{
	std::string	low;

	low = "";
	for (int i = 0; i < str.size(); i++)
		low += std::tolower((char)str[i]);
	return low;
}

bool	User::cmdPart(Session *ss, std::string const& chname, std::vector<std::string> const& sets)
{
	ChannelMap::iterator	it;
	std::string				str;

	it = _mChannels.find(makeLower(chname.substr(1)));
	if (it == _mChannels.end())
		return false;
	str = sets[0] + " " + chname;
	for (int i = 2; i < sets.size(); i++)
		str += " " + sets[i];
	it->second->broadcast(ss, str);
	it->second->removeUser(this);
	if (it->second->userCount() == 0)
	{
		delete it->second;
		Frame::instance()->removeChannel(it->first);
	}
	_mChannels.erase(it);
	return true;
}

void			User::cmdQuit(Session *ss, std::vector<std::string> const& sets, std::string const& msg)
{
	_isProperlyQuit = true;
	_pastNick.clear();
	for (ChannelMap::iterator it = _mChannels.begin(); it != _mChannels.end(); it++)
	{
		it->second->removeUser(this);
		it->second->broadcast(ss, msg);
	}
	_mChannels.clear();
}

void	User::cmdOper(void)
{
	_manager = true;
}

bool	User::isMemOfChannel(std::string const& chname) const
{
	ChannelMap::const_iterator	res;

	res = _mChannels.find(chname);
	if (res == _mChannels.end())
		return false;
	return true;
}

std::vector<std::string> User::userVector(void)
{
	ChannelMap::iterator it = _mChannels.begin();
	std::vector<std::string> res;
	std::string servername = "ft_irc";

	if (it == _mChannels.end())
	{
		res.push_back("<no Channel> " + user() + " " + host() + " " + servername + " " + nick() + " H :0 " + name());
	}
	for (it = _mChannels.begin(); it != _mChannels.end(); ++it)
	{
		res.push_back(it->second->name() + " " + user() + " " + host() + " " + servername + " " + nick() + " H :0 " + name());
	}
	return res;
}

std::vector<std::string> User::cmdWhois(void)
{
	ChannelMap::iterator it;
	std::vector<std::string> res;

	for (it = _mChannels.begin(); it != _mChannels.end(); ++it)
	{
		if (_manager)
			res.push_back(nick() + " :@" + it->second->name() + " ");
		else
			res.push_back(nick() + " :" + it->second->name() + " ");
	}
	return res;
}

void	User::setProperlyQuit(bool state)
{
	_isProperlyQuit = state;
}

void	User::partChannel(Channel *ch)
{
	_mChannels.erase(ch->name());
}
