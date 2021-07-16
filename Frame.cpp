#include "include/Frame.hpp"

Frame * Frame::pInstance = NULL;

Frame::~Frame()
{
	removeAllChannel();
}

Frame* Frame::instance()
{
	if (!pInstance)
	{
		pInstance = new Frame();
	}
	return (pInstance);
} 
void	Frame::start()
{
	server.create(80);
	while(1)
	{
		service.do_select(server);
		service.do_service(server);
	}
}

bool Frame::doesNicknameExists(std::string const& name)
{
	return (mUsers.find(name) != mUsers.end());
}

bool Frame::addUser(User *new_user)
{
	if (doesNicknameExists(new_user->nick()))
		return (false);
	mUsers[new_user->nick()] = new_user;
	return (true);
}

void	Frame::removeUser(std::string const& nick)
{
	mUsers.erase(nick);
}

bool	Frame::doesChannelExists(std::string const& name)
{
	return (mChannels.find(name) != mChannels.end());
}

void	Frame::addChannel(Channel* new_chan)
{
	if (!doesChannelExists(new_chan->name()))
		mChannels[new_chan->name()] = new_chan;
}

void	Frame::removeChannel(std::string const& name)
{
	mChannels.erase(name);
}

void	Frame::removeAllChannel()
{
	ChannelMap::iterator it;
	
	for (it = mChannels.begin() ; it != mChannels.end() ; ++it)
	{
		delete (it->second);
	}
}

ChannelMap::iterator	Frame::findChannel(std::string const& name)
{
	return mChannels.find(name);
}

void		Frame::cmdPart(Session *ss, std::vector<std::string> const& sets)
{
	int			n;
	std::string	str;

	if (sets.size() != 2)
		return "461";	//NeedMoreParams
	str = sets[1];
	while (1){
		n = str.find(",");
		if (n == std::string::npos)
			break ;
		ss->reply(doPart(ss, str.substr(0, n)));
		str = str.substr(n + 1);
	}
}

std::string		Frame::doPart(Session *ss, std::string const& sets)
{
	//if (sets.size() != 2)
	//	return "461";	//NeedMoreParams
	if (mChannels.find(sets[1]) == mChannels.end())
		return "403";	//NoSuchChannel
	if (ss->user().cmdPart(sets[1]) == false)
		return "442";	//NotOnChannel
	return "";	//Success
}

void	Frame::cmdQuit(Session *ss, std::vector<std::string> const& sets)
{
	ss->user().cmdQuit(sets);
	removeUser(ss->user().nick());
	ss->reply(ss->user().nick() + " left\n");
}

void	Frame::cmdJoin(Session *ss, std::vector<std::string> const& sets)
{
	int				n;
	std::string		str;

	if (sets.size() != 2)
		return "461";	//NeedMoreParams
	str = sets[1];
	while (1){
		n = str.find(",");
		if (std::string::npos == n)
			break ;
		ss->reply(doJoin(ss, str.substr(0, n)));
		str = str.substr(n + 1);
	}
}

std::string	Frame::doJoin(Session *ss, std::string const& sets)
{
	ChannelMap::iterator	it;
	
	//if (sets.size() < 2)
	//	return "461";	//NeedMoreParams
	if (doesChannelExists(sets[1]))
	{
		it = findChannel(sets[1]);
		it->second->addUser(&(ss->user()));
		ss->user().cmdJoin(*it);
	}
	else
		addChannel(new Channel(&(ss->user()), sets[1]));
	return ss->user().nick() + " joined to " + sets[1] + "\n";
}

void	Frame::cmdKick(Session *ss, std::vector<std::string> const& sets)
{
	//관리자 권한 확인 필요!
}

void	Frame::cmdNick(Session *ss, std::vector<std::string> const& sets)
{
	if (sets[0] == "NICK")
	{
		if (sets.size() != 2)
			return ss->reply("431");	//noNicknameGiven
		//checkNickname
		//return "432";	//ErroneusNickname
		if (doesNicknameExists(sets[1]))
			return ss->reply("433");	//NicknameInUse
		if (ss->user().addNick(sets) == false)
			return ss->reply("433");	//already be registered
	}
	else
	{
		if (doesNicknameExists(sets[2]))
			return ss->reply("433");	//ignore
		else if (&sets[0][1] != ss->user().nick())
			return ss->reply("");	//ERROR
		for (UserMap::iterator it = mUsers.begin(); it != mUsers.end(); it++)
		{
			if (it->first == ss->user().nick())
			{
				mUsers.insert(std::pair<std::string, User*>(sets[2], it->second));
				mUsers.erase(it);
				break ;
			}
		}
		ss->user().cmdNick(sets);
	}
	return ss->reply(""); //success
}

void	Frame::cmdUser(Session *ss, std::vector<std::string> const& sets)
{
	if (sets.size() != 5)
		return ss->reply("461");	//needMoreParams
	if (ss->user().cmdUser() == false)
		return ss->reply("462");	//AlreadyRegistered
	return ss->reply("");	//Success
}

void	Frame::cmdPass(Session *ss, std::vector<std::string> const& sets)
{
	if (sets.size() != 2)
		return ss->reply("461");	//NeedMoreParams
	if (server.checkPass(sets[2]))
		return ss->reply("462");	//AlreadyRegistred
	return ss->reply("");
}

void	Frame::cmdOper(Session *ss, std::vector<std::string> const& sets)
{
	if (sets.size() != 3)
		return ss->reply("461");	//NeedMoreParams
	for (UserMap::iterator it = mUsers.begin(); it != mUsers.end(); it++)
	{
		if (it->first == sets[1])
		{
			if (server.checkPass(sets[2]) == false)
				return ss->reply("464");	//passwdMismatch
			it->second->cmdOper();
			return ss->reply("381");	//RPL_YOUREOPER
		}
	}
	return ss->reply("491");	//NoOperHost
}

void	Frame::cmdTopic(Session *ss, std::vector<std::string> const& sets)
{
	ChannelMap::iterator	it;

	if (sets.size() == 2)
	{
		it = mChannels.find(sets[1]);
		if (it != mChannels.end()) 
		{
			if (it->second->topic() == "")
				return ss->reply("331");	//NoTopic
			else if (it->second->hasUser(&(ss->user())))
				return ss->reply(it->second->topic());
			return ss->reply("442");	//NotOnChannel
		}
		return ss->reply("");
	}
	else if (sets.size() == 3)
	{
		it = mChannels.find(sets[1]);
		if (it != mChannels.end())
		{
			if (it->second->isOperator(&(ss->user())) == false)
				return ss->reply("482");	//ChanOprivsNeeded
			it->second->setTopic(sets[2]);
			return ss->reply("332");	//Topic
		}
		return ss->reply("");
	}
	return ss->reply("461");	//NeedMoreParams
}

void	Frame::cmdList(Session *ss, std::vector<std::string> const& sets)
{
	int				n;
	std::string		str;

	if (sets.size() < 1 || sets.size() > 3)
		return "461";	//NeedMoreParams
	else if (sets.size() == 1)
		return ss->reply(doList(ss, ""));
	str = sets[1];
	while (1){
		n = str.find(",");
		if (std::string::npos == n)
			break ;
		ss->reply(doList(ss, str.substr(0, n)));
		str = str.substr(n + 1);
	}
}

std::string		Frame::doList(Session *ss, std::string const& sets)
{
	std::string				str;
	ChannelMap::iterator	it;

	str = "";
	if (sets == "")
	{
		for (it = mChannels.begin(); it != mChannels.end(); it++)
		{
			str += it->first + "\n";
			if (it->second->topic() != "")
				str += it->second->topic() + "\n";
		}
		if (str == "")
			return "323";	//ListEnd
		return str;
	}
	if (!(doesChannelExists(sets[1])))
		return "323";	//ListEnd
	it = mChannels.find(sets[1]);
	str += it->first + "\n";
	if (it->second->topic() != "")
		str += it->second->topic() + "\n";
	return str;
}
