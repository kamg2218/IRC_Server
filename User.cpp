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
	return (sRealname);
}

std::string		User::nick() const
{
	return (sNickname);
}

std::string		User::host() const
{
	return (sHostname);
}
std::string		User::user() const
{
	return (sUsername);
}

const ChannelMap&		User::channel() const
{
	return mChannels;
}

std::string		User::msgHeader() const
{
	return (std::string(":" + sNickname + "!" + sUsername + "@" + sHostname+ " "));
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

void	User::optionJoin(Session *ss, std::vector<std::string> const& sets)
{
	for (ChannelMap::iterator it = mChannels.begin(); it != mChannels.end(); it++)
	{
		it->second->removeUser(this);
		it->second->broadcast(ss, nick() + " left " + it->first + "\n");
		mChannels.erase(it);
		if (it->second->userCount() == 0)
			Frame::instance()->removeChannel(it->first);
	}
	ss->replyAsServer("");	//success
}

bool	User::cmdPart(Session *ss, std::string const& sets, std::string const& msg)
{
	ChannelMap::iterator	it;
	std::string				str;

	it = mChannels.find(sets.substr(1));
	if (it == mChannels.end())
		return false;
	str = nick() + " left " + it->first + "\n";
	it->second->removeUser(this);
	if (msg == "")
		it->second->broadcast(ss, str);
	else
		it->second->broadcast(ss, msg);
	mChannels.erase(it);
	if (it->second->userCount() == 0)
		Frame::instance()->removeChannel(it->first);
	return true;
}

void			User::cmdQuit(Session *ss, std::vector<std::string> const& sets)
{
	std::string	str;

	_pastNick.clear();
	for (ChannelMap::iterator it = mChannels.begin(); it != mChannels.end(); it++)
	{
		it->second->removeUser(this);
		if (sets.size() > 2)
		{
			str = sets[2].substr(1);
			for (int i = 3; i < sets.size(); i++)
				str += " " + sets[i];
			it->second->broadcast(ss, str);
		}
		else
			it->second->broadcast(ss, name() + " left\n");
	}
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
		//ss.replyAsServer("461"); //not enough parameter
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
		ss->replyAsServer("461"); //not enough parameter
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
			ss->replyAsServer("403"); //해당하는 채널없음
			return ;
		}
		if (!isMemOfChannel((*it)))
		{
			std::cout << "You are not in that channel\n";
			//ss.replyAsServer("442"); //조작하려는 유저가 해당 채널에 속하지 않음. 
			return ;
		}
		ch = (frame->findChannel((*it)));
		if (!ch->isOperator(nick()))
		{
			std::cout << "You are not the operator\n";
			//ss.replyAsServer("482"); //명령어에 대한 권한 없음
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
		//ss.replyAsServer("403"); //해당하는 채널없음
		return ;
	}
	if (isMemOfChannel(ch_name))
	{
		//ss.replyAsServer("442"); //조작하려는 유저가 해당 채널에 속하지 않음. 
		return ;
	}
	ch = Frame().findChannel(ch_name);
	if (!(*ch).second->isOperator(nick()))
	{
		//ss.replyAsServer("482"); //명령어에 대한 권한 없음
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

std::vector<std::string> User::userVector()
{
	ChannelMap::iterator it;
	std::vector<std::string> res;
	std::string servername = "ft_irc";

	for (it = mChannels.begin(); it != mChannels.end(); ++it)
	{
		res.push_back(it->second->name() + " " + user() + " " + host() + " " + servername + " " + nick() + " :0 " + name());
	}
	return (res);
}

