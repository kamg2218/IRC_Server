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
	return mChannels.find(name)->second;
}
User*	Frame::findUser(std::string const& name)
{
	return mUsers.find(name)->second;
}

bool		Frame::CheckNickname(std::string const& name)
{
	for (int i = 0; i < name.size(); i++)
	{
		if (name.size() > 9)
			return false;
		else if (name[i] == ' ' || name[i] == ',')
			return false;
		else if ((int)name[i] == 7)
			return false;
	}
	return true;
}

bool		Frame::CheckChannelname(std::string const& name)
{
	for (int i = 0; i < name.size(); i++)
	{
		if (i == 0 && name[0] != '#' && name[0] != '!' && name[0] != '&' && name[0] != '+')
			return false;
		else if (name.size() > 50)
			return false;
		else if (name[i] == ' ' || name[i] == ',')
			return false;
		else if ((int)name[i] == 7)
			return false;
	}
	return true;
}

std::string	Frame::MakeLower(std::string const& str)
{
	std::string	low;

	low = "";
	for (int i = 0; i < str.size(); i++)
		low += std::tolower((char)str[i]);
	return low;
}

void		Frame::cmdPart(Session *ss, std::vector<std::string> const& sets)
{
	int			n;
	std::string	str;

	if (sets.size() != 2)
		return ss->reply("461");	//NeedMoreParams
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
	if (mChannels.find(sets.substr(1)) == mChannels.end())
		return "403";	//NoSuchChannel
	if (ss->user().cmdPart(ss, sets.substr(1), "") == false)
		return "442";	//NotOnChannel
	return "";	//Success
}

void	Frame::cmdQuit(Session *ss, std::vector<std::string> const& sets)
{
	ss->user().cmdQuit(ss, sets);
	removeUser(ss->user().nick());
	ss->reply(ss->user().nick() + " left\n");
}

void	Frame::cmdJoin(Session *ss, std::vector<std::string> const& sets)
{
	int				n;
	std::string		str;

	if (sets.size() < 2)
		return ss->reply("461");	//NeedMoreParams
	else if (!(CheckChannelname(sets[1])))
		return ss->reply("403");	//NoSuchChannel
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
	
	if (doesChannelExists(MakeLower(sets.substr(1))))
	{
		it = mChannels.find(MakeLower(sets.substr(1)));
		it->second->addUser(&(ss->user()));
		it->second->broadcast(ss, ss->user().name() + " joined to " + it->first + "\n");
		ss->user().cmdJoin(it->second);
	}
	else
		addChannel(new Channel(&(ss->user()), MakeLower(sets.substr(1))));
	return ss->user().nick() + " joined to " + MakeLower(sets.substr(1)) + "\n";
}

void	Frame::cmdNick(Session *ss, std::vector<std::string> const& sets)
{
	if (sets[0] == "NICK")
	{
		if (sets.size() != 2)
			return ss->reply("431");	//noNicknameGiven
		else if (!(CheckNickname(sets[1])))
			return ss->reply("432");	//ErroneusNickname
		else if (doesNicknameExists(sets[1]))
			return ss->reply("433");	//NicknameInUse
		else if (ss->user().addNick(sets) == false)
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
	else if (ss->user().cmdUser() == false)
		return ss->reply("462");	//AlreadyRegistered
	return ss->reply("");	//Success
}

void	Frame::cmdPass(Session *ss, std::vector<std::string> const& sets)
{
	if (sets.size() != 2)
		return ss->reply("461");	//NeedMoreParams
	else if (server.checkPass(sets[2]))
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
	std::string				str;

	if (sets.size() < 2)
		return ss->reply("461");	//NeedMoreParams
	it = mChannels.find(MakeLower(sets[1].substr(1)));
	if (it == mChannels.end())
		return ss->reply("442");	//NotOnChannel
	if (sets.size() == 2)
	{
		if (it->second->topic() == "")
			return ss->reply("331");	//NoTopic
		else if (it->second->hasUser(&(ss->user())))
			return ss->reply(it->second->topic());
		return ss->reply("442");	//NotOnChannel
	}
	else
	{
		str = sets[2].substr(1);
		for (int i = 3; i < sets.size(); i++)
			str += " " + sets[i];
		if (it->second->isOperator(ss->user().nick()) == false)
			return ss->reply("482");	//ChanOprivsNeeded
		it->second->setTopic(str);
		return ss->reply("332");	//Topic
	}
}

void	Frame::cmdList(Session *ss, std::vector<std::string> const& sets)
{
	int				n;
	std::string		str;

	if (sets.size() < 1 || sets.size() > 3)
		return ss->reply("461");	//NeedMoreParams
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
	if (!(doesChannelExists(MakeLower(sets.substr(1)))))
		return "323";	//ListEnd
	it = mChannels.find(MakeLower(sets.substr(1)));
	str += it->first + "\n";
	if (it->second->topic() != "")
		str += it->second->topic() + "\n";
	return str;
}

#include "j_Frame.ipp"

