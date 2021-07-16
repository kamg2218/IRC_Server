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

Channel*	Frame::findChannel(std::string const& name)
{
	return ((*(mChannels.find(name))).second);
}

User*	Frame::findUser(std::string const& name)
{
	return mUsers.find(name)->second;
}

std::string		Frame::cmdPart(Session *ss, std::vector<std::string> const& sets)
{
	if (sets.size() != 2)
		return "461";	//NeedMoreParams
	else if (doesChannelExists(sets[1]) == false)
		return "403";	//NoSuchChannel
	else if (ss->user().cmdPart(ss, sets) == false)
		return "442";	//NotOnChannel
	return "";	//Success
}

void	Frame::cmdQuit(Session *ss, std::vector<std::string> const& sets)
{
	ss->user().cmdQuit(sets);
	removeUser(ss->user().nick());
}

std::string	Frame::cmdJoin(Session *ss, std::vector<std::string> const& sets)
{
	Channel*	ch;
	
	if (sets.size() < 2)
		return "461";	//NeedMoreParams
	if (doesChannelExists(sets[1]))
	{
		ch = findChannel(sets[1]);
		ch->addUser(&(ss->user()));
		ss->user().cmdJoin(std::pair<std::string, Channel*>(ch->name(), ch));
		ch->broadcast(ss, ss->user().name() + " joined " + ch->name() + "\n");
	}
	else
	{
		ch = new Channel(&(ss->user()), sets[1]);
		addChannel(ch);
		ch->broadcast(ss, ss->user().name() + " joined " + ch->name() + "\n");
	}
	return "";
}

std::string	Frame::cmdNick(Session *ss, std::vector<std::string> const& sets)
{
	//sets 확인 필요
	if (sets[0] == "NICK")
	{
		if (sets.size() != 2)
			return "431";	//noNicknameGiven
		//checkNickname
		//return "432";	//ErroneusNickname
		if (doesNicknameExists(sets[1]))
			return "433";	//NicknameInUse
		if (ss->user().addNick(sets) == false)
			return "433";	//already be registered
	}
	else
	{
		if (doesNicknameExists(sets[2]))
			return "433";	//ignore
		else if (&sets[0][1] != ss->user().nick())
			return "";	//ERROR
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
	return ""; //success
}

std::string		Frame::cmdUser(Session *ss, std::vector<std::string> const& sets)
{
	if (sets.size() != 5)
		return "461";	//needMoreParams
	if (ss->user().cmdUser() == false)
		return "462";	//AlreadyRegistered
	return "";	//Success
}

std::string		Frame::cmdPass(Session *ss, std::vector<std::string> const& sets)
{
	if (sets.size() != 2)
		return "461";	//NeedMoreParams
	if (server.checkPass(sets[2]))
		return "462";	//AlreadyRegistred
	return "";
}

std::string		Frame::cmdOper(Session *ss, std::vector<std::string> const& sets)
{
	if (sets.size() != 3)
		return "461";	//NeedMoreParams
	for (UserMap::iterator it = mUsers.begin(); it != mUsers.end(); it++)
	{
		if (it->first == sets[1])
		{
			if (server.checkPass(sets[2]) == false)
				return "464";	//passwdMismatch
			it->second->cmdOper();
			return "381";	//RPL_YOUREOPER
		}
	}
	return "491";	//NoOperHost
}

std::string		Frame::cmdTopic(Session *ss, std::vector<std::string> const& sets)
{
	Channel*	ch;

	if (sets.size() != 2 && sets.size() != 3)
		return "461";	//NeedMoreParams
	else if (!(doesChannelExists(sets[1])))
		return "";
	ch = findChannel(sets[1]);
	if (sets.size() == 2)
	{
		if (ch->topic() == "")
			return "331";	//NoTopic
		else if (ch->hasUser(&(ss->user())))
			return ch->topic();
		return "442";	//NotOnChannel
	}
	//if (sets.size() == 3)
	if (ch->isOperator(&(ss->user())) == false)
		return "482";	//ChanOprivsNeeded
	ch->setTopic(sets[2]);
	return "332";	//Topic
}

std::string		Frame::cmdList(Session *ss, std::vector<std::string> const& sets)
{
	std::string				str;
	ChannelMap::iterator	it;

	str = "";
	if (sets.size() == 1)
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
	else if (sets.size() == 2 || sets.size() == 3)
	{
		if (!(doesChannelExists(sets[1])))
			return "323";	//ListEnd
		it = mChannels.find(sets[1]);
		str += it->first + "\n";
		if (it->second->topic() != "")
			str += it->second->topic() + "\n";
		return str;
	}
	return "461";	//NeedMoreParams
}

/*
std::string		Frame::cmdInvite(Session *ss, std::vector<std::string> const& sets)
{	if (sets.size() < 3)
		return (""); //ERR_NEEDMOREPARAMS
	if (!doesNicknameExists(sets[1]))
		return (""); //ERR_NOSUCHNICK
	if (!(ss->user()->isMemOfChannel(sets[2].substr(1))))
		return (""); //ERR_NOTONCHANNEL
}*/


#include "n_Frame.ipp"
