#include "include/User.hpp"
#include "include/Session.hpp"
#include "include/Frame.hpp"

User::User()
	: didUser(false), didNick(false), is_properly_quit(false), manager(false)
{}

User::~User()
{
	if (!is_properly_quit)
	{
		Frame	*frame;
		ChannelMap::iterator it;

		frame = Frame::instance();
		it = mChannels.begin();
		for (; it != mChannels.end() ; ++it)
		{
			it->second->removeUser(this);
			it->second->broadcast(frame->findUser(sNickname), "QUIT #" + it->second->name());
		}
		//call cmdQuit
		/*
		ChannelMap::iterator it;

		it = mChannels.begin()
		for (; it != mChaneels.end() ; ++it)
		{
			(*it).removeUser(this);
			mChaneels.erase(*it);
		}
		Mainframe::instance()->removeUser(sNickName);
		Mainframe::instance()->updateChannels();
		 */
	}
}

User*			User::create()
{
	return new User();
}

void			User::setName(std::string const& s)
{
	sRealname = s;
}
void			User::setNick(std::string const& s)
{
	sNickname = s;
}
void			User::setHost(std::string const& s)
{
	sHostname = s;
}
void			User::setUser(std::string const& s)
{
	sUsername = s;
}
std::string		User::name() const
{
	return sRealname;
}

std::string		User::nick() const
{
	return sNickname;
}

std::string		User::host() const
{
	return sHostname;
}
std::string		User::user() const
{
	return sUsername;
}

const ChannelMap&		User::channel() const
{
	return mChannels;
}

std::string		User::msgHeader() const
{
	return std::string(":" + sNickname + "!" + sUsername + "@" + sHostname+ " ");
}

bool	User::CheckNick() const
{
	return didNick;
}

bool	User::CheckUser() const
{
	return didUser;
}

bool	User::IsConnected() const
{
	if (CheckNick() && CheckUser())
		return true;
	return false;
}

bool	User::CheckManager() const
{
	return manager;
}

bool	User::addNick(std::vector<std::string> const& sets)
{
	if (didNick)
		return false;	//alreadyRegistered
	sNickname = sets[1];
	didNick = true;
	return true;
}

void	User::cmdNick(std::vector<std::string> const& sets)
{
	_pastNick.insert(_pastNick.end(), sNickname);
	for (ChannelMap::iterator it = mChannels.begin(); it != mChannels.end(); it++)
		it->second->cmdNick(sNickname, sets[1]);
	sNickname = sets[1];
	return ; //success
}

bool		User::cmdUser(std::vector<std::string> const& sets)
{
	std::string	str;

	if (didUser == true)
		return false;
	didUser = true;
	setUser(sets[1]);
	str = sets[4];
	if (sets[4][0] == ':')
		str = sets[4].substr(1);
	for (int i = 4; i < sets.size(); i++)
		str += " " + sets[i];
	setName(str);
	return true;
}

void	User::cmdJoin(Channel* ch)
{
	mChannels.insert(std::pair<std::string, Channel*>(ch->name(), ch));
}

void	User::optionJoin(Session *ss, std::vector<std::string> const& sets, std::string const& msg)
{
	for (ChannelMap::iterator it = mChannels.begin(); it != mChannels.end(); it++)
	{
		it->second->removeUser(this);
		it->second->broadcast(ss, msg);
		if (it->second->userCount() == 0)
		{
			delete it->second;
			Frame::instance()->removeChannel(it->first);
		}
		mChannels.erase(it);
	}
}

std::string	User::MakeLower(std::string const& str)
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

	it = mChannels.find(MakeLower(chname.substr(1)));
	if (it == mChannels.end())
		return false;
	str = sets[0] + " " + chname;
	for (int i = 2; i < sets.size(); i++)
		str += " " + sets[i];
	it->second->removeUser(this);
	it->second->broadcast(ss, str);
	if (it->second->userCount() == 0)
	{
		delete it->second;
		Frame::instance()->removeChannel(it->first);
	}
	mChannels.erase(it);
	return true;
}

void			User::cmdQuit(Session *ss, std::vector<std::string> const& sets, std::string const& msg)
{
	_pastNick.clear();
	for (ChannelMap::iterator it = mChannels.begin(); it != mChannels.end(); it++)
	{
		it->second->removeUser(this);
		it->second->broadcast(ss, msg);
	}
	mChannels.clear();
}

void	User::cmdOper()
{
	manager = true;
}

bool	User::isMemOfChannel(std::string const& chname) const
{
	ChannelMap::const_iterator	res;

	res = mChannels.find(chname);
	if (res == mChannels.end())
		return false;
	return true;
}

std::vector<std::string> User::userVector()
{
	ChannelMap::iterator it;
	std::vector<std::string> res;
	std::string servername = "ft_irc";

	for (it = mChannels.begin(); it != mChannels.end(); ++it)
	{
		res.push_back(it->second->name() + " " + user() + " " + host() + " " + servername + " " + nick() + " :0 " + name());
	}
	return res;
}

std::vector<std::string> User::cmdWhois()
{
	ChannelMap::iterator it;
	std::vector<std::string> res;

	for (it = mChannels.begin(); it != mChannels.end(); ++it)
	{
		if (manager)
			res.push_back(nick() + " :@" + it->second->name() + " ");
		else
			res.push_back(nick() + " :" + it->second->name() + " ");
	}
	return res;
}
/*whowas
std::string User::cmdWhowas(std::string nickname, int num)
{
	std::string res;
	std::list<std::string>::reverse_iterator it;

	it = _pastNick.rbegin();
	//it = it + num;
	while (num--)
		it++;
	if (it != _pastNick.rend())
	{
		if (*it == nickname)
			res = nick() + " " + user() + " " + host() + " * :" + name();
	}
	return res;
}
*/
