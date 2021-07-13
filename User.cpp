#include "include/User.hpp"
#include "include/Session.hpp"
#include "include/Frame.hpp"

User::User(Session*	ms)
	: mysession(ms), didUser(false), didNick(false), is_properly_quit(false), manager(false)
{}

User::~User()
{
	if (!is_properly_quit)
	{
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

User*			User::create(Session *ms)
{
	return new User(ms);
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
	sHost = s;
}
std::string		User::name() const
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

std::string		User::msgHeader() const
{
	return (std::string(":" + sNickname + "!" + sNickname + "@" + sHost+ " "));
}

bool	User::CheckNick() const
{
	return didNick;
}

bool	User::CheckUser() const
{
	return didUser;
}

bool	User::CheckManager() const
{
	return manager;
}

bool	User::addNick(std::vector<std::string> const& sets)
{
	if (didNick)
		return false;
	sNickname = sets[1];
	didNick = true;
	return true;
}

void	User::cmdNick(std::vector<std::string> const& sets)
{
	_pastNick.insert(_pastNick.end(), sNickname);
	for (ChannelMap::iterator it = mChannels.begin(); it != mChannels.end(); it++)
		it->second->cmdNick(sNickname, sets[2]);
	sNickname = sets[2];
	return ; //success
}

bool		User::cmdUser()
{
	if (didUser == true)
		return false;
	didUser = true;
	return true;
}

void			User::cmdJoin(std::pair<std::string, Channel*> const& it)
{
	mChannels.insert(it);
}

bool	User::cmdPart(std::vector<std::string> const& sets)
{
	ChannelMap::iterator	it;

	it = mChannels.find(sets[1]);
	if (it == mChannels.end())
		return false;
	it->second->removeUser(this);
	mChannels.erase(it);
	return true;
}

void			User::cmdQuit(std::vector<std::string> const& sets)
{
	_pastNick.clear();
	for (ChannelMap::iterator it = mChannels.begin(); it != mChannels.end(); it++)
		it->second->removeUser(this);
	mChannels.clear();
}

void	User::cmdOper()
{
	manager = true;
}

void			User::cmdKick(std::vector<std::string> const& sets, Session *ss)
{
	ChannelMap::const_iterator	ch;

	if (sets.size() < 3)
	{
		//ss.reply("461"); //not enough parameter
		return ;
	}
	if (!Frame().doesChannelExists(sets[1]))
	{
		//ss.reply("403"); //해당하는 채널없음
		return ;
	}
	if (isMemOfChannel(sets[1]))
	{
		//ss.reply("442"); //조작하려는 유저가 해당 채널에 속하지 않음. 
		return ;
	}
	ch = Frame().findChannel(sets[1]);
	if (!(*ch).second->isOperator(nick()))
	{
		//ss.reply("482"); //명령어에 대한 권한 없음
		return ;
	}
}

bool	User::isMemOfChannel(std::string const& chname) const
{
	ChannelMap::const_iterator	res;

	res = mChannels.find(chname);
	if (res == mChannels.end())
		return (false);
	return (true);
}

