#include "include/User.hpp"
#include "include/Session.hpp"
#include "include/Frame.hpp"

User::User(void)
	: _didNick(false), _didUser(false), _manager(false), _password(true), _isProperlyQuit(false)
{
}

User::User(User const& other)
{
	*this = other;
}

User&	User::operator=(User const& other)
{
	if (this == &other)
		return *this;
	return *this;
}

User::~User(void)
{
	if (isRegistered() && !_isProperlyQuit)
	{
		Frame		*frame;
		ChannelMap::iterator		it;
		std::vector<std::string>	quitmsg;

		quitmsg.push_back("QUIT");
		quitmsg.push_back(":User unexpectedly lost connection");
		frame = Frame::instance();
		frame->cmdQuit(frame->findUser(_sNickname), quitmsg);
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

std::string		User::msgHeader(void) const
{
	return std::string(_sNickname + "!" + _sUsername + "@" + _sHostname);
}

bool			User::checkNick(void) const
{
	return _didNick;
}
bool			User::checkUser(void) const
{
	return _didUser;
}

/*
   * Check both Nick and User
 */
bool			User::isRegistered(void) const
{
	if (checkNick() && checkUser())
		return true;
	return false;
}
bool			User::checkManager(void) const
{
	return _manager;
}

/*
   * Restore Nick name for connection
 */
bool			User::addNick(std::vector<std::string> const& sets)
{
	if (_didNick)
		return false;	//alreadyRegistered
	_sNickname = sets[1];
	_didNick = true;
	return true;
}

/*
   * Change Nick name
 */
void			User::cmdNick(std::vector<std::string> const& sets)
{
	_pastNick.insert(_pastNick.end(), _sNickname);
	for (ChannelMap::iterator it = _mChannels.begin(); it != _mChannels.end(); it++)
		it->second->cmdNick(_sNickname, sets[1]);
	_sNickname = sets[1];
	return ;
}

bool			User::cmdUser(std::vector<std::string> const& sets)
{
	std::string	str;
	std::string::size_type	i;

	if (_didUser == true)
		return false;
	_didUser = true;
	setUser(sets[1]);
	str = sets[4];
	if (sets[4][0] == ':')
		str = sets[4].substr(1);
	for (i = 5; i < sets.size(); i++)
		str += " " + sets[i];
	setName(str);
	return true;
}

void			User::cmdJoin(Channel* ch)
{
	_mChannels.insert(std::pair<std::string, Channel*>(ch->name(), ch));
}

void			User::optionJoin(Session *ss, std::string const& msg)
{
	ChannelMap::iterator	it;

	for (it = _mChannels.begin(); it != _mChannels.end(); it++)
	{
		it->second->broadcast(ss, msg);
		it->second->cmdPart(this->nick());
		if (it->second->userCount() == 0)
		{
			delete it->second;
			Frame::instance()->removeChannel(it->first);
		}
		_mChannels.erase(it);
	}
}

/*
   * Change Upper characters to Lower
 */
std::string		User::makeLower(std::string const& str)
{
	std::string	low;
	std::string::size_type	i;

	low = "";
	for (i = 0; i < str.size(); i++)
		low += std::tolower((char)str[i]);
	return low;
}

bool			User::cmdPart(Session *ss, std::string const& chname, std::vector<std::string> const& sets)
{
	std::string				str;
	std::string::size_type	i;
	ChannelMap::iterator	it;

	it = _mChannels.find(makeLower(chname.substr(1)));
	if (it == _mChannels.end())
		return false;
	str = sets[0] + " " + chname;
	for (i = 2; i < sets.size(); i++)
		str += " " + sets[i];
	it->second->broadcast(ss, str);
	it->second->cmdPart(this->nick());
	if (it->second->userCount() == 0)
	{
		delete it->second;
		Frame::instance()->removeChannel(it->first);
	}
	_mChannels.erase(it);
	return true;
}

void			User::cmdQuit(std::vector<std::string> const& sets)
{
	ChannelMap::iterator	it;
	ChannelMap::iterator	tt;
	std::string				str;
	std::string::size_type	i;

	_isProperlyQuit = true;
	for (i = 1; i < sets.size(); i++)
		str += " " + sets[i];
	for (it = _mChannels.begin(); it != _mChannels.end(); )
	{
		tt = it++;
		tt->second->cmdPart(this->nick());
		if (tt->second->userCount() == 0)
		{
			delete tt->second;
			Frame::instance()->removeChannel(tt->first);
		}
		_mChannels.erase(tt);
	}
	_didNick = false;
	_didUser = false;
}

void			User::cmdOper(void)
{
	_manager = true;
}

bool			User::isMemOfChannel(std::string const& chname) const
{
	ChannelMap::const_iterator	res;

	res = _mChannels.find(chname);
	if (res == _mChannels.end())
		return false;
	return true;
}

std::vector<std::string>	User::userVector(void)
{
	ChannelMap::iterator it = _mChannels.begin();
	std::vector<std::string> res;
	std::string servername = "ft_irc";

	// has not Channel
	if (it == _mChannels.end())
	{
		if (_manager)
			res.push_back("<no Channel> " + user() + " " + host() + " " + servername + " " + nick() + " H @ :0 " + name());
		else
			res.push_back("<no Channel> " + user() + " " + host() + " " + servername + " " + nick() + " H :0 " + name());

	}
	for (it = _mChannels.begin(); it != _mChannels.end(); ++it)
	{
		if (_manager)
			res.push_back(it->second->name() + " " + user() + " " + host() + " " + servername + " " + nick() + " H @ :0 " + name());
		else
			res.push_back(it->second->name() + " " + user() + " " + host() + " " + servername + " " + nick() + " H :0 " + name());
	}
	return res;
}


std::vector<std::string>	User::cmdWhois(void)
{
	ChannelMap::iterator it;
	std::vector<std::string> res;

	for (it = _mChannels.begin(); it != _mChannels.end(); ++it)
	{
		if (it->second->hasOper(it->first))
			res.push_back(nick() + " :@" + it->second->name() + " ");
		else
			res.push_back(nick() + " :" + it->second->name() + " ");
	}
	return res;
}

void			User::setProperlyQuit(bool state)
{
	_isProperlyQuit = state;
}

void			User::partChannel(Channel *ch)
{
	_mChannels.erase(ch->name());
}
