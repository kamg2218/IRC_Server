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
	this->_mUsers = other._mUsers;
	this->_mChannels = other._mChannels;
	this->_server = other._server;
	this->_service = other._service;
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
void		Frame::start(base const& bs)
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

bool		Frame::addUser(Session *new_user)
{
	if (doesNicknameExists(new_user->user().nick()))
		return false;
	_mUsers[new_user->user().nick()] = new_user;
	return true;
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
		if (name[i] == ' ' || name[i] == ',')
			return false;
		else if ((int)name[i] == 7)
			return false;
	}
	return true;
}

bool		Frame::checkChannelname(std::string const& name)
{
	std::string::size_type	i;

	for (i = 0; i < name.size(); i++)
	{
		if (name.size() > 51 || name.size() <= 1)
			return false;
		else if (i == 0 && name[0] != '#'
				&& name[0] != '!' && name[0] != '&' && name[0] != '+')
			return false;
		else if (name[i] == ' ' || name[i] == ',')
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
		else if (_mChannels.find(makeLower(v[i].substr(1))) == _mChannels.end())
			ss->err403(v[i].substr(1));	//NoSuchChannel
		else if (ss->user().cmdPart(ss, v[i], sets) == false)
			ss->err442(v[i].substr(1));	//NotOnChannel
	}
}

void		Frame::cmdQuit(Session *ss, std::vector<std::string> const& sets)
{
	std::string	msg;

	msg = vectorToString(sets);
	broadcastAll(ss, msg);
	ss->user().cmdQuit(sets);
	removeUser(ss->user().nick());
	_server.users().erase(ss->soc().sd());
	ss->replyAsUser(ss, msg);
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
	UserMap::iterator	it;

	if (sets.size() < 2)
		return ss->err431();			//noNicknameGiven
	else if (ss->user().pass() == false)
		return ;
	else if (!(checkNickname(sets[1]))
			|| (sets[1][0] == ':' && sets[1].size() == 1))
		return ss->err432(sets[1]);	//ErroneusNickname
	else if (doesNicknameExists(sets[1]))
		return ss->err433(sets[1]);	//NicknameInUse
	else if (ss->user().addNick(sets) == true)
		return ;						//success to register
	else								//change Nickname
	{
		it = _mUsers.find(ss->user().nick());
		if (it == _mUsers.end())
			return ;
		for (UserMap::iterator tmp = _mUsers.begin(); tmp != _mUsers.end(); tmp++)
			ss->replyAsUser(tmp->second, vectorToString(sets));
		_mUsers.insert(std::pair<std::string, Session*>(sets[1], it->second));
		_mUsers.erase(it);
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
	else if (addUser(ss) == false)
		return ss->err462();			//AlreadyRegistered
	ss->user().setHost(inet_ntoa(ss->soc().sin().sin_addr));
	ss->rep001(&(ss->user()));	//Success
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

	if (sets.size() < 2)
		return ss->err461("TOPIC");			//NeedMoreParams
	else if (!(checkChannelname(sets[1])))
		return ss->err461("TOPIC");			//NeedMoreParams
	it = _mChannels.find(makeLower(sets[1].substr(1)));
	if (it == _mChannels.end())
		return ss->err442(sets[1].substr(1));	//NotOnChannel
	if (sets.size() == 2 || sets[2][0] != ':')
	{
		if (it->second->topic() == "")
			return ss->rep331(it->first);		//NoTopic
		else if (it->second->hasUser(ss->user().nick()))
			return ss->rep332(it->first, it->second->topic());
		return ss->err442(it->first);			//NotOnChannel
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
			ss->rep322(it->second);			//List
		return ss->rep323();					//ListEnd
	}
	v = split_comma(sets[1]);
	for (i = 0; i < v.size(); i++)
	{
		ss->rep321();							//ListStart
		if (checkChannelname(v[i])
				&& doesChannelExists(makeLower(v[i].substr(1))))
		{
			it = _mChannels.find(makeLower(v[i].substr(1)));
			ss->rep322(it->second);
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
		if (cmd.size() < 3 || (!checkChannelname(sets[1])))
			ss->err461("KICK");
		else if (!doesChannelExists(makeLower(cmd[1].substr(1))))
			ss->err403(cmd[1].substr(1));
		else if (!(ss->user().isMemOfChannel(cmd[1].substr(1))))
			ss->err442(cmd[1].substr(1));
		else if (!((channel = findChannel(makeLower(cmd[1].substr(1))))->isOperator(ss->user().nick())))
			ss->err482(cmd[1].substr(1));
		else if (ss->user().nick() != cmd[2] && channel->hasUser(cmd[2]))
		{
			target = findUser(cmd[2]);
			channel->broadcast(ss, vectorToString(cmd));
			target->user().partChannel(channel);
			channel->cmdPart(target->user().nick());
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

std::vector<std::string>		Frame::getMask(std::string const& str)
{
	int		wild;
	std::vector<std::string>	v;
	std::string::size_type		i;

	wild = -1;
	for (i = 0; i < str.size(); i++)
	{
		if (str[i] == '*')
			wild = i;
	}
	//checkChannelName
	for (ChannelMap::iterator it = _mChannels.begin(); it != _mChannels.end(); it++)
	{
		if (checkMask(makeLower(str), it->first, wild))
			v.insert(v.end(), it->first);
	}
	if (v.size() != 0)
		return v;
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
    std::string res;
	std::vector<std::string>::size_type i = 0;

    while (i < sets.size() - 1)
    {
        res += sets[i] + " ";
		i++;
	}
	res += sets[i];
    return res;
}

std::string		Frame::vectorToStringpriv(std::vector<std::string> const& sets)
{
    std::string res;

    for (std::vector<std::string>::size_type i = 0; i < sets.size(); i++)
    {
		if (i == 2 && sets[2][0] != ':')
			res += ":";
        res += sets[i] + " ";
	}
	return res.substr(0, res.size() - 1);
}

MainServer&		Frame::getServer()
{
	return _server;
}

void		Frame::cmdWho(Session *ss, std::vector<std::string> const& sets)
{
	UserMap::iterator itu;

	if (sets.size() == 1 || (sets.size() == 2 && sets[1] == "*"))
	{
		for (itu = _mUsers.begin(); itu != _mUsers.end(); ++itu)
			ss->rep352(itu->second->user().userVector());
		ss->rep315("");
	}
	else
	{
		std::vector<std::string> v;
		if (checkChannelname(sets[1]))
			v = getMask(sets[1].substr(1));
		else
			v = getMask(sets[1]);
		for (std::vector<std::string>::size_type i = 0; i < v.size(); i++)
		{
			if (doesChannelExists(v[i]))	// channel
			{
				Channel *channel;
				channel = findChannel(v[i]);
				if (sets.size() > 2 && sets[2] == "o")
					ss->rep352(channel->channeloperVector());
				else
					ss->rep352(channel->channelVector());
			}
			else
			{
				for (itu = _mUsers.begin(); itu != _mUsers.end(); ++itu)
				{
					if (itu->second->user().host() == v[i]			// hostname
							||	itu->second->user().name() == v[i]	// realname
							||	itu->second->user().nick() == v[i])	// nickname
					{
						if (sets.size() > 2 && sets[2] == "o" && itu->second->user().checkManager() == 0)
							continue ;
						ss->rep352(itu->second->user().userVector());
					}
				}
			}
		}
		ss->rep315(sets[1]);
	}
}

void		Frame::cmdPrivmsg(Session *ss, std::vector<std::string> const& sets)
{
    std::vector<std::string> receivers;
    std::string receiver;
	std::string msg;

	if (sets.size() == 1)
        return ss->err411(sets[0]);   // errNORECIPIENT
	if (sets.size() != 3)
	{
		if (sets[1][0] == ':')
        	return ss->err411(sets[0]);   // errNORECIPIENT
		if (sets[2][0] != ':')
        	return ss->err412();   // errNOTEXTTOSEND
	}
    // split receivers
    receivers = split_comma(sets[1]);
    std::vector<std::string>::iterator		receiverit = receivers.begin();
    std::vector<std::string>::iterator		tmp;
    
    // check receivers
    while (receiverit != receivers.end())
    {
        if (checkChannelname(*receiverit))
		{
            if (!doesChannelExists(makeLower((*receiverit).substr(1))))
                return ss->err404(*receiverit);   // errCANNOTSENDTOCHAN
        }
        else
        {
            if (!doesNicknameExists(*receiverit))
                return ss->err401(*receiverit);       // errNOSUCHNICK
			for (tmp = receiverit + 1; tmp != receivers.end(); tmp++)
            {
                if (*(tmp - 1) == *tmp)
                    return ss->err407(*tmp);       // errTOOMANYTARGETS
            }
        }
        receiverit++;
    }
	for (std::vector<std::string>::size_type i = 2 ;i < sets.size(); i++)
		msg += sets[i];
    for (receiverit = receivers.begin(); receiverit != receivers.end(); receiverit++)
    {
        receiver = *receiverit;
        if (receiver[0] == '#')
        {
			Channel *channel;

			channel = findChannel(makeLower(receiver.substr(1)));
            channel->privmsgBroadcast(ss, "PRIVMSG " + receiver + " " + msg);
        }
        else
        {
			Session *session;

			session = findUser(receiver);
			ss->replyAsUser(session, "PRIVMSG " + receiver + " " + msg);
        }
    }
}

void		Frame::cmdWhois(Session *ss, std::vector<std::string> const& sets)
{
	std::vector<std::string>::iterator		it;
	std::vector<std::string>::iterator		it2;
	std::vector<std::string>		v;
	UserMap::iterator		itu;
	std::string		res;
	std::vector<std::string>		resline;

	if (sets.size() == 1)
		return ss->err431();
	v = split_comma(sets[1]);
	for (std::vector<std::string>::size_type i = 0; i < v.size(); i++)
	{
		if (!doesNicknameExists(v[i]))
		{
			ss->err401(v[i]);
			return ss->rep318(sets[1]);
		}
	}
	for (it = v.begin(); it != v.end(); it++)
	{
		Session *session;

		session = findUser(*it);
		ss->rep311(session);
		if (session->user().checkManager())
			ss->rep313(session);
		resline = session->user().User::cmdWhois();
		for (it2 = resline.begin(); it2 != resline.end(); it2++)
			ss->rep319(*it2);
	}
	ss->rep318(sets[1]);
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
	ss->replyAsServer(res + "PRIVMSG <receiver>[,<receiver>] <text to be send>");

	ss->replyAsServer(res + "JOIN <channel>[,<channel>] [<key>[,<key>]");
	ss->replyAsServer(res + "PART <channel>[,<channel>]");
	ss->replyAsServer(res + "TOPIC <channel> [<topic>]");
	ss->replyAsServer(res + "LIST [<channel>[,<channel>]]");
	ss->replyAsServer(res + "INVITE <nick> <channel>");
	ss->replyAsServer(res + "KICK <channel> <user> [<comment>]");

	ss->replyAsServer(res + "NICK <nick>");
	ss->replyAsServer(res + "USER <user> <host> <server> <real>");
	ss->replyAsServer(res + "PASS <password>");
	ss->replyAsServer(res + "OPER <user> <password>");
	ss->replyAsServer(res + "QUIT <Quit message>");

}

void	Frame::cmdPong(Session *ss)
{
	ss->setTime(std::time(0));
}
