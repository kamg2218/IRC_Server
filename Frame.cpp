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

void	Frame::cmdPart(Session *ss, std::vector<std::string> const& sets)
{
	ss->user().cmdPart(sets);
}

void	Frame::cmdQuit(Session *ss, std::vector<std::string> const& sets)
{
	ss->user().cmdQuit(sets);
	removeUser(ss->user().nick());
}

void	Frame::cmdJoin(Session *ss, std::vector<std::string> const& sets)
{
	ChannelMap::iterator	it;
	
	if (doesChannelExists(sets[1]))
	{
		it = findChannel(sets[1]);
		it->second->addUser(&(ss->user()));
		ss->user().cmdJoin(*it);
	}
	else
	{
		addChannel(new Channel(&(ss->user()), sets[1]));
	}
}

void	Frame::cmdKick(Session *ss, std::vector<std::string> const& sets)
{
	//관리자 권한 확인 필요!
}

void	Frame::cmdNick(Session *ss, std::vector<std::string> const& sets)
{
	//sets 확인 필요
	if (sets[0] == "NICK")
	{
		if (ss->user().addNick(sets) == false)
			return ;	//already be registered
	}
	else
	{
		if (doesNicknameExists(sets[2]))
			return ;	//ignore
		else if (&sets[0][1] != ss->user().nick())
			return ;	//ERROR
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
	return ; //success
}

void	Frame::cmdUser(Session *ss, std::vector<std::string> const& sets)
{
	if (sets[0] != "USER")
		return ; //서버 USER 명령어
	if (ss->user().cmdUser() == false)
		return ; //NICK 명령어 필요
}
