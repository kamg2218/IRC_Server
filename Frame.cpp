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

bool Frame::addUser(Session *new_user)
{
	if (doesNicknameExists(new_user->user().nick()))
		return (false);
	mUsers[new_user->user().nick()] = new_user;
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
	ChannelMap::iterator tmp;
	
	for (it = mChannels.begin() ; it != mChannels.end() ;)
	{
		tmp = it;
		it++;
		delete (tmp->second);
		mChannels.erase(tmp);
	}
}

Channel*	Frame::findChannel(std::string const& name)
{
	return mChannels.find(name)->second;
}
Session*	Frame::findUser(std::string const& name)
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
	std::string	re;
	std::string	str;

	if (sets.size() < 2)
		return ss->Err_461("PART");	//NeedMoreParams
	re = "";
	if (sets.size() > 2)
	{
		re = sets[2];
		if (re[0] == ':')
			re = re.substr(1);
		for (int i = 3; i < sets.size(); i++)
			re += " " + sets[i];
	}
	str = sets[1];
	while (1){
		n = str.find(",");
		doPart(ss, str.substr(0, n), re);
		if (n == std::string::npos)
			break ;
		str = str.substr(n + 1);
	}
}

void	Frame::doPart(Session *ss, std::string const& sets, std::string const& re)
{
	if (mChannels.find(sets.substr(1)) == mChannels.end())
		return ss->Err_403(sets.substr(1));	//NoSuchChannel
	else if (ss->user().cmdPart(ss, sets.substr(1), re) == false)
		return ss->Err_442(sets.substr(1));	//NotOnChannel
	ss->replyAsServer("");	//Success
}

void	Frame::cmdQuit(Session *ss, std::vector<std::string> const& sets)
{
	std::string	msg;

	msg = ss->user().nick() + " left";
	ss->user().cmdQuit(ss, sets);
	removeUser(ss->user().nick());
	if (sets.size() > 1 && sets[1][0] == ':')
	{
		msg = sets[1].substr(1);
		for (int i = 2; i < sets.size(); i++)
			msg += " " + sets[i];
	}
	ss->replyAsServer(msg);
}

void	Frame::cmdJoin(Session *ss, std::vector<std::string> const& sets)
{
	int				n;
	std::string		str;

	if (sets.size() < 2)
		return ss->Err_461("JOIN");	//NeedMoreParams
	else if (sets.size() == 2 && sets[1] == "O")
		return ss->user().optionJoin(ss, sets);
	str = sets[1];
	while (1){
		n = str.find(",");
		if (!(CheckChannelname(str.substr(0, n))))
			return ss->Err_403(str.substr(0, n));	//NoSuchChannel
		ss->replyAsServer(doJoin(ss, str.substr(0, n)));
		if (std::string::npos == n)
			break ;
		str = str.substr(n + 1);
	}
}
//reply check
std::string	Frame::doJoin(Session *ss, std::string const& sets)
{
	ChannelMap::iterator	it;
	
	if (doesChannelExists(MakeLower(sets.substr(1))))
	{
		it = mChannels.find(MakeLower(sets.substr(1)));
		it->second->addUser(ss);
		it->second->broadcast(ss, ss->user().nick() + " joined to " + it->first + "\n");
		ss->user().cmdJoin(it->second);
		//Topic
	}
	else
		addChannel(new Channel(ss, MakeLower(sets.substr(1))));
	return ss->Rep_353(MakeLower(sets.substr(1)), ss->user().nick());	//RPL_NAMREPLY
}

void	Frame::cmdNick(Session *ss, std::vector<std::string> const& sets)
{
	if (sets[0] == "NICK")
	{
		if (sets.size() != 2)
			return ss->Err_431();	//noNicknameGiven
		else if (!(CheckNickname(sets[1])))
			return ss->Err_432(sets[1]);	//ErroneusNickname
		else if (doesNicknameExists(sets[1]))
			return ss->Err_433(sets[1]);	//NicknameInUse
		else if (ss->user().addNick(sets) == false)
			return ss->Err_433(sets[1]);	//already be registered
	}
	else
	{
		if (doesNicknameExists(sets[2]))
			return ss->Err_433(sets[2]);	//NicknameInUse
		else if (sets[0].substr(1) != ss->user().nick())
			return ss->Err_432(sets[0].substr(1));	//ErroneusNickname
		for (UserMap::iterator it = mUsers.begin(); it != mUsers.end(); it++)
		{
			if (it->first == ss->user().nick())
			{
				mUsers.insert(std::pair<std::string, Session*>(sets[2], it->second));
				mUsers.erase(it);
				break ;
			}
		}
		ss->user().cmdNick(sets);
	}
	return ss->replyAsServer(""); //success
}

void	Frame::cmdUser(Session *ss, std::vector<std::string> const& sets)
{
	if (sets.size() < 5)
		return ss->Err_461("USER");	//needMoreParams
	else if (ss->user().cmdUser(sets) == false)
		return ss->Err_462();	//AlreadyRegistered
	else if (addUser(ss) == false)
		return ss->Err_462();	//AlreadyRegistered
	return ss->replyAsServer("");	//Success
}

void	Frame::cmdPass(Session *ss, std::vector<std::string> const& sets)
{
	if (sets.size() != 2)
		return ss->Err_461("PASS");	//NeedMoreParams
	else if (server.checkPass(sets[2]))
		return ss->Err_462();	//AlreadyRegistred
	return ss->replyAsServer("");
}

void	Frame::cmdOper(Session *ss, std::vector<std::string> const& sets)
{
	if (sets.size() != 3)
		return ss->Err_461("OPER");	//NeedMoreParams
	for (UserMap::iterator it = mUsers.begin(); it != mUsers.end(); it++)
	{
		if (it->first == sets[1])
		{
			if (server.checkPass(sets[2]) == false)
				return ss->Err_464();	//passwdMismatch
			it->second->user().cmdOper();
			return ss->Rep_381();	//RPL_YOUREOPER
		}
	}
	return ss->Err_491();	//NoOperHost
}

void	Frame::cmdTopic(Session *ss, std::vector<std::string> const& sets)
{
	ChannelMap::iterator	it;
	std::string				str;

	if (sets.size() < 2)
		return ss->Err_461("TOPIC");	//NeedMoreParams
	it = mChannels.find(MakeLower(sets[1].substr(1)));
	if (it == mChannels.end())
		return ss->Err_442(sets[1].substr(1));	//NotOnChannel
	if (sets.size() == 2)
	{
		if (it->second->topic() == "")
			return ss->Rep_331(it->first);	//NoTopic
		else if (it->second->hasUser(&(ss->user())))
			return ss->replyAsServer(it->second->topic());
		return ss->Err_442(it->first);	//NotOnChannel
	}
	else
	{
		str = sets[2].substr(1);
		for (int i = 3; i < sets.size(); i++)
			str += " " + sets[i];
		if (it->second->isOperator(ss->user().nick()) == false)
			return ss->Err_482(it->first);	//ChanOprivsNeeded
		it->second->setTopic(str);
		return ss->Rep_332(it->first, str);	//Topic
	}
}

void	Frame::cmdList(Session *ss, std::vector<std::string> const& sets)
{
	int				n;
	std::string		str;

	if (sets.size() < 1)
		return ss->Err_461("LIST");	//NeedMoreParams
	else if (sets.size() == 1)
		return ss->replyAsServer(doList(ss, ""));
	str = sets[1];
	while (1){
		n = str.find(",");
		ss->replyAsServer(doList(ss, str.substr(0, n)));
		if (std::string::npos == n)
			break ;
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
			return ss->Rep_323();	//ListEnd
		return str;
	}
	if (!(doesChannelExists(MakeLower(sets.substr(1)))))
		return ss->Rep_323();	//ListEnd
	it = mChannels.find(MakeLower(sets.substr(1)));
	str += it->first + "\n";
	if (it->second->topic() != "")
		str += it->second->topic() + "\n";
	return str;
}

std::vector<std::string> split_comma(std::string s)
{
	std::vector<std::string>	res;
	std::string::size_type		pos;

	while ((pos = s.find(",")) != std::string::npos)
	{
		res.push_back(s.substr(0, pos));
		s.erase(0, pos + 1);
	}
	res.push_back(s);
	return (res);
}

std::vector<std::vector<std::string> > kicklist(std::vector<std::string> const& sets)
{
	std::string					message;
	std::vector<std::string>	chlist;
	std::vector<std::string>	tgtlist;
	std::vector<std::string>	subres;
	std::vector<std::vector<std::string> >	res;

	chlist = split_comma(sets[1]);
	tgtlist = split_comma(sets[2]);
	for (int i = 3 ; i < sets.size() ; i++)
	{
		if (i == 3 && sets[i][0] != ':')
			break ;
		message += sets[i];
	}
	for (int i = 0 ; i < chlist.size() || i < tgtlist.size() ; i++)
	{
		if (chlist.size() == 1)
			subres.push_back(chlist[0]);
		else if (i < chlist.size())
			subres.push_back(chlist[i]);
		if (i < tgtlist.size())
			subres.push_back(tgtlist[i]);
		subres.push_back(message);
		res.push_back(subres);
		subres.clear();
	}
	return (res);
}

void	Frame::cmdKick(Session *ss, std::vector<std::string> const& sets)
{
	std::vector<std::vector<std::string> >	cmdsets;
	std::vector<std::string>	cmd;
	Session	*target;
	Channel *channel;

	if (sets.size() < 3)
		ss->Err_461("KICK"); //ERR_NEEDMOREPARAMS
	cmdsets = kicklist(sets);
	while(cmdsets.size())
	{
		cmd = cmdsets[0];
		if (cmd.size() < 2)
			ss->Err_461("KICK"); //ERR_NEEDMOREPARAMS
		else if (cmd[0].find("#") == std::string::npos || cmd[0].find("&") == std::string::npos)
			ss->Err_461("KICK"); //ERR_NEEDMOREPARAMS
		else if (!doesChannelExists(cmd[0].substr(1)))
			ss->replyAsServer("403"); //ERR_NOSUCHCHANNEL
		else if (!(ss->user().isMemOfChannel(cmd[0].substr(1))))
			ss->replyAsServer("442"); //ERR_NOTONCHANNEL;
		else if (!((channel = findChannel(cmd[0].substr(1)))->isOperator(ss->user().nick())))
			ss->replyAsServer("482"); //ERR_CHANOPRIVSNEEDED
		else
		{
			if (doesNicknameExists(cmd[1]))
			{
				target = findUser(cmd[1]);
				channel->broadcast(ss, cmd[0] + cmd[1] + cmd[2]);
				target->user().cmdPart(ss, cmd[0], cmd[2]);
			}
		}
		cmdsets.erase(cmdsets.begin());
	}
}

void	Frame::cmdInvite(Session *ss, std::vector<std::string> const& sets)
{
	Channel *channel;
	Session	*target;
	std::string	rpl;
	std::string	joinmsg;

	if (sets.size() < 3)
	{
		ss->replyAsServer("461"); // NOT ENOUGH PARAM
		return ;
	}
	if (sets[1].find("#") == std::string::npos && sets[1].find("&") == std::string::npos)
		ss->replyAsServer("461"); //NOT ENOUGH PARAM
	else if (!doesChannelExists(sets[1].substr(1)))
		ss->replyAsServer("403"); //NO SUCH CHANNEL
	else if (!(ss->user().isMemOfChannel(sets[1].substr(1))))
		ss->replyAsServer("442"); //ERR_NOTONCHANNEL;
	else if (!doesNicknameExists(sets[2]))
		ss->replyAsServer("401"); //ERR_NOSUCHNICK
	else if (!(target = findUser(sets[2]))->user().isMemOfChannel(channel->name()))
		ss->replyAsServer("443"); //ERR_USERONCHANNEL;
	else
	{
		rpl.append("341 ");
		rpl.append(channel->name());
		rpl.append(" ");
		rpl.append(target->user().nick());
		ss->replyAsServer(rpl);
		target->replyAsServer(rpl);
		joinmsg.append("JOIN ");
		joinmsg.append("#");
		joinmsg.append(channel->name());
		target->user().cmdJoin(channel); 
		channel->addUser(target);
		channel->broadcast(target, joinmsg);
	}
}

bool		Frame::checkMask(std::string const& str, std::string const& name, int wild)
{
	int		s;

	s = str.size() - wild - 1;
	if (str.substr(0, wild) != name.substr(0, wild))
		return false;
	else if (str.substr(wild + 1, s) != name.substr(name.size() - s, s))
		return false;
	return true;
}

std::vector<std::string>	Frame::getMask(std::string const& str)
{
	int		wild;
	std::vector<std::string>	v;

	wild = -1;
	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == '*')
			wild = i;
	}
	//checkChannelName
	for (ChannelMap::iterator it = mChannels.begin(); it != mChannels.end(); it++)
	{
		if (checkMask(str, it->first, wild))
			v.insert(v.end(), it->first);
	}
	if (v.size() != 0)
		return v;
	//checkServerName - Not Exist
	for (UserMap::iterator it = mUsers.begin(); it != mUsers.end(); it++)
	{
		//checkUserNickName
		if (checkMask(str, it->first, wild))
			v.insert(v.end(), it->first);
		//checkUserHost
		else if (checkMask(str, it->second->user().host(), wild))
			v.insert(v.end(), it->first);
		//checkUserRealName
		else if (checkMask(str, it->second->user().name(), wild))
			v.insert(v.end(), it->first);
	}
	return v;
}
