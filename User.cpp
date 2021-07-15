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
	Frame *	frame;
	std::vector<std::string> chlist;
	std::vector<std::string> targetlist;
	std::vector<std::string>::const_iterator it;
	std::string	ch_name;
	std::vector<std::string> partsets;
	//ChannelMap::const_iterator	ch;
	Channel	*ch;

	std::cout << "HERE\n";
	for (std::vector<std::string>::const_iterator it = sets.begin(); it != sets.end() ; ++it)
	{
		std::cout << (*it) << std::endl;
	}
	std::cout << "size : " << sets.size() << " END\n";
	frame = Frame().instance();
	if (sets.size() < 3)
	{
		std::cout << "Not enough parameter\n";
		//ss.reply("461"); //not enough parameter
		return ;
	}
	std::cout << "< check channel\n";
	for (it = sets.begin() ; it != sets.end(); ++it)
	{

		if ((*it).find("#") != std::string::npos || (*it).find("&") != std::string::npos)
			chlist.push_back((*it).substr(1));
		else if ((*it) != "KICK")
			break;
	}
	if (chlist.empty())
	{
		std::cout << "Not enough parameter\n";
		ss->reply("461"); //not enough parameter
		return ;
	}
	for (; it != sets.end(); ++it)
	{
		if ((*it).find(":") == std::string::npos)	//마지막 :를 확인해야함.
			break;
		targetlist.push_back((*it));
	}
	for (it = chlist.begin() ; it != chlist.end() ; ++it)
	{
		if (!frame->doesChannelExists((*it)))
		{
			std::cout << "There is no such channel\n";
			ss->reply("403"); //해당하는 채널없음
			return ;
		}
		if (!isMemOfChannel((*it)))
		{
			std::cout << "You are not in that channel\n";
			//ss.reply("442"); //조작하려는 유저가 해당 채널에 속하지 않음. 
			return ;
		}
		ch = (frame->findChannel((*it)));
		if (!ch->isOperator(nick()))
		{
			std::cout << "You are not the operator\n";
			//ss.reply("482"); //명령어에 대한 권한 없음
			return ;
		}
		// user Part
		partsets.push_back("PART");
		for (std::vector<std::string>::size_type i = 0 ; i < targetlist.size() ; i++)
		{
			if (frame->doesNicknameExists(targetlist[i]))
			{
				partsets.push_back(targetlist[i]);
				//frame->findUser(targetlist[i])->cmdPart(ss, partsets);
				partsets.pop_back();
			}
		}
	}
/*
	ch_name = sets[1].substr(1);
	if (!Frame().doesChannelExists(ch_name))
	{
		//ss.reply("403"); //해당하는 채널없음
		return ;
	}
	if (isMemOfChannel(ch_name))
	{
		//ss.reply("442"); //조작하려는 유저가 해당 채널에 속하지 않음. 
		return ;
	}
	ch = Frame().findChannel(ch_name);
	if (!(*ch).second->isOperator(nick()))
	{
		//ss.reply("482"); //명령어에 대한 권한 없음
		return ;
	}
	*/
	// 실행
	//it.second->removeUser(this);
	//mChannels.erase(it);
}

bool	User::isMemOfChannel(std::string const& chname) const
{
	ChannelMap::const_iterator	res;

	res = mChannels.find(chname);
	if (res == mChannels.end())
		return (false);
	return (true);
}

