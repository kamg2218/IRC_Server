#include "include/Frame.hpp"

Frame * Frame::_pInstance = NULL;

Frame::Frame(void)
{
}

Frame::Frame(Frame const& other)
{
	*this = other;
}

Frame&	Frame::operator=(Frame const& other)
{
	if (this == &other)
		return *this;
	return *this;
}

Frame::~Frame(void)
{
	removeAllUser();
	removeAllChannel();
}

Frame*		Frame::instance(void)
{
	if (!_pInstance)
		_pInstance = new Frame();
	return _pInstance;
}

void		Frame::start(Base const& bs)
{
	_server.create(bs);
	while(1)
	{
		_service.doSelect(_server);
		_service.doService(_server);
	}
}

bool		Frame::doesNicknameExists(std::string const& name)
{
	return _mUsers.find(name) != _mUsers.end();
}

void		Frame::removeUser(std::string const& nick)
{
	_mUsers.erase(nick);
}

bool		Frame::doesChannelExists(std::string const& name)
{
	return _mChannels.find(name) != _mChannels.end();
}

void		Frame::addChannel(Channel* new_chan)
{
	if (!doesChannelExists(new_chan->name()))
		_mChannels[new_chan->name()] = new_chan;
}

void		Frame::removeChannel(std::string const& name)
{
	_mChannels.erase(name);
}

void		Frame::removeAllUser(void)
{
	UserMap::iterator it;
	UserMap::iterator tmp;
	
	for (it = _mUsers.begin() ; it != _mUsers.end() ;)
	{
		tmp = it;
		it++;
		delete (tmp->second);
		_mUsers.erase(tmp);
	}
}

void		Frame::removeAllChannel()
{
	ChannelMap::iterator it;
	ChannelMap::iterator tmp;
	
	for (it = _mChannels.begin() ; it != _mChannels.end() ;)
	{
		tmp = it;
		it++;
		delete (tmp->second);
		_mChannels.erase(tmp);
	}
}

Channel*		Frame::findChannel(std::string const& name)
{
	return _mChannels.find(name)->second;
}

Session*	Frame::findUser(std::string const& name)
{
	return _mUsers.find(name)->second;
}

bool		Frame::checkNickname(std::string const& name)
{
	std::string::size_type	i;

	if (name.size() == 0 || name.size() > 9)
		return false;
	for (i = 0; i < name.size(); i++)
	{
		if (name[i] == ' ' || name[i] == ',' || name[i] == '*')
			return false;
		else if ((int)name[i] == 7)
			return false;
	}
	return true;
}

bool		Frame::checkChannelname(std::string const& name)
{
	std::string::size_type	i;

	if (name.size() > 51 || name.size() <= 1)
		return false;
	for (i = 0; i < name.size(); i++)
	{
		if (i == 0 && name[0] != '#'
				&& name[0] != '!' && name[0] != '&' && name[0] != '+')
			return false;
		else if (name[i] == ' ' || name[i] == ',' || name[i] == '*')
			return false;
		else if ((int)name[i] == 7)
			return false;
	}
	return true;
}

/*
   * Change Upper characters to Lower
*/
std::string		Frame::makeLower(std::string const& str)
{
	std::string	low;
	std::string::size_type	i;

	low = "";
	for (i = 0; i < str.size(); i++)
		low += std::tolower((char)str[i]);
	return low;
}

void		Frame::cmdPart(Session *ss, std::vector<std::string> const& sets)
{
	std::string::size_type		i;
	std::vector<std::string>	v;

	if (sets.size() < 2)
		return ss->err461("PART");	//NeedMoreParams
	v = split_comma(sets[1]);
	for (i = 0; i < v.size(); i++)
	{
		if (checkChannelname(v[i]) == false)
			ss->err461("PART");	//NeedMoreParams
		else if (!doesChannelExists(makeLower(v[i].substr(1))))
			ss->err403(v[i].substr(1));	//NoSuchChannel
		else if (ss->user().cmdPart(ss, v[i], sets) == false)
			ss->err442(v[i].substr(1));	//NotOnChannel
	}
}

void		Frame::cmdQuit(Session *ss, std::vector<std::string> const& sets)
{
	broadcastAll(ss, vectorToString(sets));
	ss->user().cmdQuit();
	removeUser(ss->user().nick());
}

void		Frame::cmdJoin(Session *ss, std::vector<std::string> const& sets)
{
	std::string::size_type		i;
	std::vector<std::string>	v;

	if (sets.size() < 2)
		return ss->err461("JOIN");	//NeedMoreParams
	else if (sets.size() == 2 && sets[1] == "O")
		return ss->user().optionJoin(ss, vectorToString(sets));
	v = split_comma(sets[1]);
	for (i = 0; i < v.size(); i++)
	{
		if (!(checkChannelname(v[i])))
			ss->err461("JOIN");	//NeedMoreParams
		else
			doJoin(ss, sets, v[i]);
	}
}

void		Frame::doJoin(Session *ss, std::vector<std::string> const& sets, std::string const& chname)
{
	std::string::size_type		i;
	std::vector<std::string>	v;
	std::string					msg;
	Channel*					ch;

	msg = sets[0];
	msg += " " + chname;
	for (i = 2; i < sets.size(); i++)
		msg += " " + sets[i];
	if (doesChannelExists(makeLower(chname.substr(1))))
		ch = findChannel(makeLower(chname.substr(1)));
	else
	{
		ch = new Channel(ss, makeLower(chname.substr(1)));
		addChannel(ch);
	}
	ch->addUser(ss);
	ch->broadcast(ss, msg);
	ss->user().cmdJoin(ch);
	ch->cmdJoin(ss);
}

void		Frame::cmdNick(Session *ss, std::vector<std::string> const& sets)
{
	if (sets.size() < 2)
		return ss->err431();			//noNicknameGiven
	else if (ss->user().pass() == false)
		return ;
	else if (!(checkNickname(sets[1])))
		return ss->err432(sets[1]);		//ErroneusNickname
	else if (doesNicknameExists(sets[1]))
		return ss->err433(sets[1]);		//NicknameInUse
	else if (ss->user().addNick(sets) == true)
		return ;						//success to register
	else								//change Nickname
	{
		broadcastAll(ss, vectorToString(sets));
		ss->replyAsUser(ss, vectorToString(sets));
		_mUsers.insert(std::pair<std::string, Session*>(sets[1], ss));
		_mUsers.erase(ss->user().nick());
		ss->user().cmdNick(sets);
	}
}

void		Frame::cmdUser(Session *ss, std::vector<std::string> const& sets)
{
	if (sets.size() < 5)
		return ss->err461("USER");		//needMoreParams
	else if (ss->user().pass() == false)
		return ;
	else if (ss->user().checkNick() == false)
		return ;
	else if (ss->user().cmdUser(sets) == false)
		return ss->err462();			//AlreadyRegistered
	ss->user().setHost(inet_ntoa(ss->soc().sin().sin_addr));
	_mUsers[ss->user().nick()] = ss;
	ss->rep001(&(ss->user()));			//Success
	printCommand(ss);
}

void		Frame::cmdPass(Session *ss, std::vector<std::string> const& sets)
{
	if (sets.size() != 2)
		return ss->err461("PASS");		//NeedMoreParams
	else if (ss->user().isRegistered())
		return ss->err462();			//AlreadyRegistered
	else if (_server.checkPass(sets[1]))
		ss->user().setPass(true);
	else
		ss->user().setPass(false);
}

void		Frame::cmdTopic(Session *ss, std::vector<std::string> const& sets)
{
	ChannelMap::iterator	it;
	std::string				str;
	std::string::size_type	i;

	if (sets.size() < 2 || !(checkChannelname(sets[1])))
		return ss->err461("TOPIC");					//NeedMoreParams
	it = _mChannels.find(makeLower(sets[1].substr(1)));
	if (it == _mChannels.end())
		return ss->err442(sets[1].substr(1));		//NotOnChannel
	else if (!(it->second->hasUser(ss->user().nick())))
		return ss->err442(it->first);				//NotOnChannel
	if (sets.size() == 2 || sets[2][0] != ':')
	{
		if (it->second->topic() == "")
			return ss->rep331(it->first);			//NoTopic
		ss->rep332(it->first, it->second->topic());	//Topic
	}
	else
	{
		str = sets[2].substr(1);
		for (i = 3; i < sets.size(); i++)
			str += " " + sets[i];
		it->second->cmdTopic(ss, str, vectorToString(sets));
	}
}

void		Frame::cmdList(Session *ss, std::vector<std::string> const& sets)
{
	std::string::size_type		i;
	ChannelMap::iterator		it;
	std::vector<std::string>	v;

	if (sets.size() == 1)
	{
		ss->rep321();							//ListStart
		for (it = _mChannels.begin(); it != _mChannels.end(); it++)
			ss->rep322(it->second);				//List
		return ss->rep323();					//ListEnd
	}
	v = split_comma(sets[1]);
	for (i = 0; i < v.size(); i++)
	{
		ss->rep321();							//ListStart
		if (doesChannelExists(makeLower(v[i].substr(1))))
		{
			it = _mChannels.find(makeLower(v[i].substr(1)));
			ss->rep322(it->second);				//List
		}
		ss->rep323();							//ListEnd
	}
}

std::vector<std::string>		Frame::split_comma(std::string s)
{
	std::vector<std::string>	res;
	std::string::size_type		pos;

	while ((pos = s.find(",")) != std::string::npos)
	{
		res.push_back(s.substr(0, pos));
		if (res.size() == pos + 1)
			break ;
		s.erase(0, pos + 1);
	}
	res.push_back(s);
	return res;
}

std::vector<std::vector<std::string> >		Frame::kicklist(std::vector<std::string> const& sets)
{
	std::string					message;
	std::vector<std::string>	chlist;
	std::vector<std::string>	tgtlist;
	std::vector<std::string>	subres;
	std::vector<std::vector<std::string> >	res;

	chlist = split_comma(sets[1]);
	tgtlist = split_comma(sets[2]);
	for (std::vector<std::string>::size_type i = 3 ; i < sets.size() ; i++)
	{
		if (i == 3 && sets[i].find(":") == std::string::npos)
			break ;
		message += sets[i];
		if (i != sets.size()- 1)
			message += " ";
	}
	for (std::vector<std::string>::size_type i = 0 ; i < chlist.size() || i < tgtlist.size() ; i++)
	{
		subres.push_back("KICK");
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
	return res;
}

void		Frame::cmdKick(Session *ss, std::vector<std::string> const& sets)
{
	Session		*target;
	Channel		*channel;
	std::vector<std::string>		cmd;
	std::vector<std::vector<std::string> >	cmdsets;

	cmdsets = kicklist(sets);
	while(cmdsets.size())
	{
		cmd = cmdsets[0];
		if (cmd.size() < 3 || (!checkChannelname(cmd[1])))
			ss->err461("KICK");
		else if (!doesChannelExists(makeLower(cmd[1].substr(1))))
			ss->err403(cmd[1].substr(1));
		else if (!(ss->user().isMemOfChannel(cmd[1].substr(1))))
			ss->err442(cmd[1].substr(1));
		else if (!((channel = findChannel(makeLower(cmd[1].substr(1))))->hasOper(ss->user().nick())))
			ss->err482(cmd[1].substr(1));
		else if (ss->user().nick() != cmd[2] && channel->hasUser(cmd[2]))
		{
			target = findUser(cmd[2]);
			channel->broadcast(ss, vectorToString(cmd));
			target->user().partChannel(channel);
			channel->cmdPart(target->user().nick());
			if (!channel->userCount())
			{
				delete channel;
				removeChannel(channel->name());
			}
		}
		cmdsets.erase(cmdsets.begin());
	}
}

void		Frame::cmdInvite(Session *ss, std::vector<std::string> const& sets)
{
	Channel		*channel;
	Session		*target;
	std::vector<std::string>	joincmd;

	if (sets.size() < 3 || (!checkChannelname(sets[2])))
		ss->err461("INVITE");
	else if (!doesChannelExists(sets[2].substr(1)))
		ss->err403(sets[2].substr(1));
	else if (!(ss->user().isMemOfChannel(sets[2].substr(1))))
		ss->err442(sets[2].substr(1));
	else if (!doesNicknameExists(sets[1]))
		ss->err401(sets[1]);
	else
	{
		target = findUser(sets[1]);
		channel = findChannel(makeLower(sets[2].substr(1)));
		if (target->user().isMemOfChannel(channel->name()))
			ss->err443(sets[1], channel->name());
		else
		{
			ss->rep341(channel->name(), target->user().nick());
			target->rep341(channel->name(), target->user().nick());
			joincmd.push_back("JOIN");
			joincmd.push_back("#" + channel->name());
			cmdJoin(target, joincmd);
		}
	}
}

bool		Frame::checkMask(std::string const& str, std::string const& name, int wild)
{
	std::string::size_type	s;

	if (wild == -1 && str != name)
		return false;
	else if (wild != -1)
	{
		s = str.size() - wild - 1;
		if (str.substr(0, wild) != name.substr(0, wild))
			return false;
		else if (name.size() < s)
			return false;
		else if (str.substr(wild + 1, s) != name.substr(name.size() - s, s))
			return false;
	}
	return true;
}

std::vector<std::string>		Frame::channelMask(std::string const& str)
{
	int		wild;
	std::vector<std::string>	v;
	std::string::size_type		i;

	wild = -1;
	for (i = 0; i < str.size(); i++)
	{
		if (str[i] == '*')
		{
			wild = i;
			break ;
		}
	}
	//checkChannelName
	for (ChannelMap::iterator it = _mChannels.begin(); it != _mChannels.end(); it++)
	{
		if (checkMask(makeLower(str), it->first, wild))
			v.insert(v.end(), it->first);
	}
	if (v.size() == 0)
		return userMask(str);
	return v;
}

std::vector<std::string>		Frame::userMask(std::string const& str)
{
	int		wild;
	std::vector<std::string>	v;
	std::string::size_type		i;

	wild = -1;
	for (i = 0; i < str.size(); i++)
	{
		if (str[i] == '*')
		{
			wild = i;
			break ;
		}
	}
	//checkServerName - Not Exist
	for (UserMap::iterator it = _mUsers.begin(); it != _mUsers.end(); it++)
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

std::string		Frame::vectorToString(std::vector<std::string> const& sets)
{
    std::string							res;
	std::vector<std::string>::size_type i;

	i = 0;
    while (i < sets.size() - 1)
    {
        res += sets[i] + " ";
		i++;
	}
	res += sets[i];
    return res;
}

MainServer&		Frame::getServer()
{
	return _server;
}

void		Frame::cmdWho(Session *ss, std::vector<std::string> const& sets)
{
	UserMap::iterator			itu;
	std::vector<std::string>	v;
	
	if (sets.size() == 1)
		v = channelMask("*");
	else
		v = channelMask(sets[1]);
	if (v.size() == 0)
		return ss->rep315(sets[1]);		// RPL_ENDOFWHO
	for (std::vector<std::string>::size_type i = 0; i < v.size(); i++)
	{
		// check channelName
		if (doesChannelExists(v[i]))
		{
			Channel *channel;
			channel = findChannel(v[i]);
			if (sets.size() > 2 && sets[2] == "o")
				ss->rep352(channel->channeloperVector());		// RPL_WHOREPLY
			else
				ss->rep352(channel->channelVector());			// RPL_WHOREPLY
			ss->rep315("#" + v[i]);		// RPL_ENDOFWHO
		}
		else
		{
			// check hostName
			for (itu = _mUsers.begin(); itu != _mUsers.end(); ++itu)
			{
				if (itu->second->user().host() == v[i])
					ss->rep352(itu->second->user().userVectorOper(sets));	// RPL_WHOREPLY
			}
			// check realName
			for (itu = _mUsers.begin(); itu != _mUsers.end(); ++itu)
			{
				if (itu->second->user().name() == v[i])
					ss->rep352(itu->second->user().userVectorOper(sets));	// RPL_WHOREPLY
			}
			// check nickName
			for (itu = _mUsers.begin(); itu != _mUsers.end(); ++itu)
			{
				if (itu->second->user().nick() == v[i])
					ss->rep352(itu->second->user().userVectorOper(sets));	// RPL_WHOREPLY
			}
			ss->rep315(v[i]);	// RPL_ENDOFWHO
		}
	}
}

void		Frame::cmdPrivmsg(Session *ss, std::vector<std::string> const& sets)
{
    std::vector<std::string>							receivers;
	std::string		name;
	std::set<std::string>								s;
	std::set<std::string>::iterator						is;
	std::pair<std::set<std::string>::iterator, bool>	ret;
	std::string											msg;
	Channel												*channel;
	Session												*session;

	if (sets.size() == 1)
		return ss->err411(sets[0]);		// ERRNORECIPIENT
	else if (sets.size() == 2 || sets[2][0] != ':')
		return ss->err412();			// ERRNOTEXTTOSEND

	// massage
	for (std::vector<std::string>::size_type i = 2 ;i < sets.size(); i++)
		msg += " " + sets[i];
    
	// split receivers
    receivers = split_comma(sets[1]);
    std::vector<std::string>::iterator		receiverit = receivers.begin();
   
    // check receivers
	for (; receiverit != receivers.end(); receiverit++)
	{
		name.clear();
		if (checkChannelname(*receiverit))
			name = makeLower((*receiverit));
		else
			name = *receiverit;
		ret = s.insert(name);
		if (ret.second == false)
			ss->err407(*receiverit);		// ERRTOOMANYTARGETS
		else if(checkChannelname(*receiverit))
		{
            if (!doesChannelExists(makeLower((*receiverit).substr(1))))
                ss->err404((*receiverit).substr(1));   	// ERRCANNOTSENDTOCHAN
			else
			{
				channel = findChannel(makeLower((*receiverit).substr(1)));
            	channel->privmsgBroadcast(ss, "PRIVMSG " + *receiverit + " " + msg.substr(1));
			}
		}
		else if (checkNickname(*receiverit) == false)
			ss->err411(sets[0]);   			// ERRNORECIPIENT
		else if (!doesNicknameExists(*receiverit))
			ss->err401(*receiverit);       // ERRNOSUCHNICK
		else
		{
			session = findUser(*receiverit);
			ss->replyAsUser(session, "PRIVMSG " + *receiverit + " " + msg.substr(1));
		}
	}
}

void		Frame::cmdWhois(Session *ss, std::vector<std::string> const& sets)
{
	Session								*session;
	std::vector<std::string>			split_v;
	std::vector<std::string>			wild_v;
	std::vector<std::string>::iterator	its;
	std::vector<std::string>::iterator	itw;

	if (sets.size() == 1)
		return ss->err431();			// ERR_NONICKNAMEGIVEN
	split_v = split_comma(sets[1]);
	for (its = split_v.begin(); its != split_v.end(); ++its)
	{
		wild_v = userMask(*its);
		if (wild_v.size() == 0)
		{
			ss->err401(*its);			// ERR_NOSUCHNICK
			continue ;
		}
		for (itw = wild_v.begin(); itw != wild_v.end(); ++itw)
		{
			session = findUser(*itw);
			ss->rep311(session);			// RPL_WHOISUSER
			if (session->user().checkManager())
				ss->rep313(session);		// RPL_WHOISOPERATOR
			session->user().cmdWhois(ss);	// RPL_WHOISCHANNELS
			ss->rep318(*itw);				// RPL_ENDOFWHOIS
		}
	}
}

void		Frame::broadcastAll(Session *ss, std::string const& str)
{
	UserMap::iterator		it;

	it = _mUsers.begin();
	for (; it != _mUsers.end(); ++it)
	{
		if (it->second != ss)
			ss->replyAsUser(it->second, str);
	}
}

void		Frame::printCommand(Session *ss)
{
	std::string res;

	res += "PRIVMSG ";
	res += ss->user().nick();
	res += " :";
	ss->replyAsServer(res + "<Command>");
	ss->replyAsServer(res + "WHO <nick> [o]");
	ss->replyAsServer(res + "WHOIS <nickmask>[,<nickmask>]");
	ss->replyAsServer(res + "PRIVMSG <receiver>[,<receiver>] <:text to be send>");

	ss->replyAsServer(res + "JOIN <channel>[,<channel>]");
	ss->replyAsServer(res + "PART <channel>[,<channel>]");
	ss->replyAsServer(res + "TOPIC <channel> [<topic>]");
	ss->replyAsServer(res + "LIST [<channel>[,<channel>]]");
	ss->replyAsServer(res + "INVITE <nick> <channel>");
	ss->replyAsServer(res + "KICK <channel> <user> [<comment>]");
	ss->replyAsServer(res + "KICK <channel> <user>[,<user>] [<comment>]");
	ss->replyAsServer(res + "KICK <channel>[,<channel>] <user>[,<user>] [<comment>]");

	ss->replyAsServer(res + "NICK <nick>");
	ss->replyAsServer(res + "USER <user> <host> <server> <real>");
	ss->replyAsServer(res + "PASS <password>");
	ss->replyAsServer(res + "OPER <user> <password>");
	ss->replyAsServer(res + "QUIT [Quit message]");

}

void	Frame::cmdPong(Session *ss)
{
	ss->setPing(false);
	ss->setTime(std::time(0));
}
