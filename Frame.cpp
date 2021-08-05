cmdPart(Session *ss, std::vector<std::string> const& sets)
{
	std::vector<std::string>	v;

	if (sets.size() < 2)
		return ss->Err_461("PART");	//NeedMoreParams
	v = split_comma(sets[1]);
	for (int i = 0; i < v.size(); i++)
	{
		if (CheckChannelname(v[i]) == false)
			ss->Err_461("PART");	//NeedMoreParams
		else if (mChannels.find(MakeLower(v[i].substr(1))) == mChannels.end())
			ss->Err_403(v[i].substr(1));	//NoSuchChannel
		else if (ss->user().cmdPart(ss, v[i], sets) == false)
			ss->Err_442(v[i].substr(1));	//NotOnChannel
	}
}

void	Frame::cmdQuit(Session *ss, std::vector<std::string> const& sets)
{
	std::string	msg;

	msg = vectorToString(sets);
	ss->user().cmdQuit(ss, sets, msg);
	removeUser(ss->user().nick());
	server.users().erase(ss->soc().sd());
	ss->replyAsServer(msg);
}

void	Frame::cmdJoin(Session *ss, std::vector<std::string> const& sets)
{
	std::vector<std::string>	v;

	if (sets.size() < 2)
		return ss->Err_461("JOIN");	//NeedMoreParams
	else if (sets.size() == 2 && sets[1] == "O")
		return ss->user().optionJoin(ss, sets, vectorToString(sets));
	v = split_comma(sets[1]);
	for (int i = 0; i < v.size(); i++)
	{
		if (!(CheckChannelname(v[i])))
		{
			ss->Err_461("JOIN");	//NeedMoreParams
			continue ;
		}
		doJoin(ss, sets, v[i]);
	}
}

void	Frame::doJoin(Session *ss, std::vector<std::string> const& sets, std::string const& chname)
{
	std::vector<std::string>	v;
	std::string					msg;
	Channel*					ch;

	msg = sets[0];
	msg += " " + chname;
	for (int i = 2; i < sets.size(); i++)
		msg += " " + sets[i];
	if (doesChannelExists(MakeLower(chname.substr(1))))
		ch = findChannel(MakeLower(chname.substr(1)));
	else
	{
		ch = new Channel(ss, MakeLower(chname.substr(1)));
		addChannel(ch);
	}
	ch->addUser(ss);
	ch->broadcast(ss, msg);
	ss->user().cmdJoin(ch);
	ch->cmdJoin(ss);
	printcommand(ss);
}

void	Frame::cmdNick(Session *ss, std::vector<std::string> const& sets)
{
	UserMap::iterator	it;

	if (sets.size() < 2)
		return ss->Err_431();	//noNicknameGiven
	else if (ss->user().pass() == false)
		return ;
	else if (!(CheckNickname(sets[1])))
		return ss->Err_432(sets[1]);	//ErroneusNickname
	else if (doesNicknameExists(sets[1]))
		return ss->Err_433(sets[1]);	//NicknameInUse
	else if (ss->user().addNick(sets) == true)
		return ;	//success to register
	else			//change Nickname
	{
		it = mUsers.find(ss->user().nick());
		if (it == mUsers.end())
			return ;
		for (UserMap::iterator tmp = mUsers.begin(); tmp != mUsers.end(); tmp++)
		{
			if (tmp->first == it->first)
				continue ;
			ss->replyAsUser(tmp->second, vectorToString(sets));
		}
		ss->replyAsServer(vectorToString(sets));
		mUsers.insert(std::pair<std::string, Session*>(sets[1], it->second));
		mUsers.erase(it);
		ss->user().cmdNick(sets);
	}
}

void	Frame::cmdUser(Session *ss, std::vector<std::string> const& sets)
{
	if (sets.size() < 5)
		return ss->Err_461("USER");	//needMoreParams
	else if (ss->user().pass() == false)
		return ;
	else if (ss->user().CheckNick() == false)
		return ;	//ignore
	else if (ss->user().cmdUser(sets) == false)
		return ss->Err_462();	//AlreadyRegistered
	else if (addUser(ss) == false)
		return ss->Err_462();	//AlreadyRegistered
	ss->user().setHost(inet_ntoa(ss->soc().sin().sin_addr));
	return ss->Rep_001(&(ss->user()));	//Success
}

void	Frame::cmdPass(Session *ss, std::vector<std::string> const& sets)
{
	if (sets.size() != 2)
		return ss->Err_461("PASS");	//NeedMoreParams
	else if (ss->user().IsConnected())
		return ss->Err_462();		//AlreadyRegistered
	else if (server.checkPass(sets[1]))
	{
		ss->user().setPass(true);
		ss->setPing(true);
	}
	else
		ss->user().setPass(false);
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
	else if (!(CheckChannelname(sets[1])))
		return ss->Err_461("TOPIC");	//NeedMoreParams
	it = mChannels.find(MakeLower(sets[1].substr(1)));
	if (it == mChannels.end())
		return ss->Err_442(sets[1].substr(1));	//NotOnChannel
	if (sets.size() == 2 || sets[2][0] != ':')
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
	ChannelMap::iterator		it;
	std::vector<std::string>	v;

	if (sets.size() == 1)
	{
		ss->Rep_321();	//ListStart
		for (it = mChannels.begin(); it != mChannels.end(); it++)
			ss->Rep_322(it->second);	//List
		return ss->Rep_323();	//ListEnd
	}
	v = split_comma(sets[1]);
	for (int i = 0; i < v.size(); i++)
	{
		ss->Rep_321();	//ListStart
		if (CheckChannelname(v[i])
				&& doesChannelExists(MakeLower(v[i].substr(1))))
		{
			it = mChannels.find(MakeLower(v[i].substr(1)));
			ss->Rep_322(it->second);
		}
		ss->Rep_323();	//ListEnd
	}
}

std::vector<std::string>	Frame::split_comma(std::string s)
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

std::vector<std::vector<std::string> >	Frame::kicklist(std::vector<std::string> const& sets)
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
	return (res);
}

void	Frame::cmdKick(Session *ss, std::vector<std::string> const& sets)
{
	std::vector<std::vector<std::string> >	cmdsets;
	std::vector<std::string>	cmd;
	Session	*target;
	Channel *channel;

	cmdsets = kicklist(sets);
	while(cmdsets.size())
	{
		cmd = cmdsets[0];
		if (cmd.size() < 3)
			ss->Err_461("KICK"); //ERR_NEEDMOREPARAMS
		else if (cmd[1].find("#") == std::string::npos && cmd[1].find("&") == std::string::npos)
			ss->Err_461("KICK"); //ERR_NEEDMOREPARAMS
		else if (!doesChannelExists(MakeLower(cmd[1].substr(1))))
			ss->Err_403(cmd[1].substr(1)); //ERR_NOSUCHCHANNEL
		else if (!(ss->user().isMemOfChannel(cmd[1].substr(1))))
			ss->Err_442(cmd[1].substr(1)); //ERR_NOTONCHANNEL;
		else if (!((channel = findChannel(MakeLower(cmd[1].substr(1))))->isOperator(ss->user().nick())))
			ss->Err_482(cmd[1].substr(1)); //ERR_CHANOPRIVSNEEDED
		else
		{
			if (doesNicknameExists(cmd[2]))
			{
				target = findUser(cmd[2]);
				channel->broadcast(ss, vectorToString(cmd));
				//target->user().cmdPart(ss, cmd[0], cmd[2]);
			}
		}
		cmdsets.erase(cmdsets.begin());
	}
}

void	Frame::cmdInvite(Session *ss, std::vector<std::string> const& sets)
{
	Channel *channel;
	Session	*target;
	std::vector<std::string>	joincmd;
	
	if (sets.size() < 3)
		ss->Err_461("INVITE"); // NOT ENOUGH PARAM
	else if (sets[2].find("#") == std::string::npos && sets[2].find("&") == std::string::npos)
		ss->Err_461("INVITE"); // NOT ENOUGH PARAM
	else if (!doesChannelExists(sets[2].substr(1)))
		ss->Err_403(sets[2].substr(1)); //NO SUCH CHANNEL
	else if (!(ss->user().isMemOfChannel(sets[2].substr(1))))
		ss->Err_442(sets[2].substr(1)); //ERR_NOTONCHANNEL;
	else if (!doesNicknameExists(sets[1]))
		ss->Err_401(sets[1]); //ERR_NOSUCHNICK
	else
	{
		target = findUser(sets[1]);
		channel = findChannel(sets[2].substr(1));
		if (target->user().isMemOfChannel(channel->name()))
			ss->Err_443(sets[1], channel->name()); //ERR_USERONCHANNEL;
		else
		{
			ss->Rep_341(channel->name(), target->user().nick());
			target->Rep_341(channel->name(), target->user().nick());
			joincmd.push_back("JOIN");
			joincmd.push_back("#" + channel->name());
			cmdJoin(target, joincmd);
		}
	}
}

bool		Frame::checkMask(std::string const& str, std::string const& name, int wild)
{
	int		s;

	if (wild == -1 && str != name)
		return false;
	else if (wild != -1)
	{
		s = str.size() - wild - 1;
		if (str.substr(0, wild) != name.substr(0, wild))
			return false;
		else if (str.substr(wild + 1, s) != name.substr(name.size() - s, s))
			return false;
	}
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
		if (checkMask(MakeLower(str), it->first, wild))
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

std::string Frame::vectorToString(std::vector<std::string> const& sets)
{
    std::string res;
    int i = 0;

    while (i < sets.size() - 1)
    {
        res += sets[i] + " ";
		i++;
	}
	res += sets[i];
    return res;
}

std::string Frame::vectorToStringpriv(std::vector<std::string> const& sets)
{
	/*
    std::string res;
	int check = 0;

	if (sets[2][0] != ':')
		check = 1;
    for (int i = 0; i < sets.size() - 1; i++)
    {
		if (i == 2 && check)
			res += ":";
        res += sets[i] + " ";
	}
	if (i == 2 && check)
		res += ":";
	res += sets[i];
    return res;
	*/
    std::string res;

    for (int i = 0; i < sets.size(); i++)
    {
		if (i == 2 && sets[2][0] != ':')
			res += ":";
        res += sets[i] + " ";
	}
	return res.substr(0, res.size() - 1);
}

MainServer&	Frame::GetServer()
{
	return (server);
}

void	Frame::cmdWho(Session *ss, std::vector<std::string> const& sets)
{
	UserMap::iterator itu;

	if (sets.size() == 1 || (sets.size() == 2 && sets[1] == "*"))
	{
		for (itu = mUsers.begin(); itu != mUsers.end(); ++itu)
			ss->Rep_352(itu->second->user().userVector());
		ss->Rep_315("");
	}
	else
	{
		std::vector<std::string> v;
		if (CheckChannelname(sets[1]))
			v = getMask(sets[1].substr(1));
		else
			v = getMask(sets[1]);
		for (int i = 0; i < v.size(); i++)
		{
			if (doesChannelExists(v[i]))	// channel
			{
				Channel *channel;
				channel = findChannel(v[i]);
				if (sets.size() > 2 && sets[2] == "o")
					ss->Rep_352(channel->channeloperVector());
				else
					ss->Rep_352(channel->channelVector());
				/*
				if (sets.size() < 3)
					ss->Rep_352(channel->channelVector());
				else if (sets[2] == "o")
					ss->Rep_352(channel->channeloperVector());
				else
					ss->Rep_352(channel->channelVector());
					*/
			}
			else
			{
				for (itu = mUsers.begin(); itu != mUsers.end(); ++itu)
				{
					if (itu->second->user().host() == v[i]			// hostname
							||	itu->second->user().name() == v[i]	// realname
							||	itu->second->user().nick() == v[i])	// nickname
					{
						if (sets.size() > 2 && sets[2] == "o" && itu->second->user().CheckManager() == 0)
							continue ;
						ss->Rep_352(itu->second->user().userVector());
					}
					else
						break ;
				}
			}
		}
		ss->Rep_315(sets[1]);
	}
}

void		Frame::cmdPrivmsg(Session *ss, std::vector<std::string> const& sets)
{
    std::vector<std::string> receivers;
    std::string receiver;

	if (sets.size() == 1)
        return (ss->Err_411(sets[0]));   // ERR_NORECIPIENT
	if (sets.size() != 3)
	{
		std::cout << "size > 3\n";
		std::cout << "sets[1][0] : " << sets[1][0] << std::endl;
		std::cout << "sets[2][0] : " << sets[2][0] << std::endl;
		std::cout << "sets[2] : " << sets[2] << std::endl;
		if (sets[1][0] == ':')
		{
			std::cout << "come!!!\n";
        	return (ss->Err_411(sets[0]));   // ERR_NORECIPIENT
		}
		if (sets[2][0] != ':')
		{
			std::cout << "come!!!\n";
        	return (ss->Err_412());   // ERR_NOTEXTTOSEND
		}
	}
    // receiver에 콤마로 split해서 저장하기
    receivers = split_comma(sets[1]);
    std::vector<std::string>::iterator receiverit = receivers.begin();
    std::vector<std::string>::iterator tmp;
    
    // 메세지 전까지 확인하기
    while (receiverit != receivers.end())
    {
        //if (sets[1][0] == '#')  // channel
        if (CheckChannelname(*receiverit))
		{
            if (!doesChannelExists(MakeLower((*receiverit).substr(1))))
                return (ss->Err_404(*receiverit));   // ERR_CANNOTSENDTOCHAN
        }
        else
        {
            if (!doesNicknameExists(*receiverit))
                return (ss->Err_401(*receiverit));       // ERR_NOSUCHNICK
			for (tmp = receiverit + 1; tmp != receivers.end(); tmp++)
            {
                if (*(tmp - 1) == *tmp)
                    return (ss->Err_407(*tmp));       // ERR_TOOMANYTARGETS
            }
        }
        receiverit++;
    }
    for (receiverit = receivers.begin(); receiverit != receivers.end(); receiverit++)
    {
        receiver = *receiverit;
        if (receiver[0] == '#')
        {
			Channel *channel;
			channel = findChannel(MakeLower(receiver.substr(1)));
            channel->broadcast(ss, vectorToStringpriv(sets));
        }
        else
        {
			Session *session;
			session = findUser(receiver);
			ss->replyAsUser(session, vectorToStringpriv(sets));
        }
    }
}

void	Frame::cmdWhois(Session *ss, std::vector<std::string> const& sets)
{
	std::vector<std::string>::iterator it;
	std::vector<std::string>::iterator it2;
	std::vector<std::string> v;
	Usermap::iterator itu;
	std::string res;
	std::vector<std::string> resline;

	if (sets.size() == 1)
		return ss->Err_431();
	v = split_comma(sets[1]);
	for (int i = 0; i < v.size(); i++)
	{
		if (!doesNicknameExists(v[i]))
		{
			ss->Err_401(v[i]);
			return (ss->Rep_318(sets[1]));
		}
	}
	for (it = v.begin(); it != v.end(); it++)
	{
		Session *session;
		int check;

		session = findUser(*it);
		ss->Rep_311(session);
		if (session->user().CheckManager())
			ss->Rep_313(session);
		resline = session->user().User::cmdWhois();	// check!
		for (it2 = resline.begin(); it2 != resline.end(); it2++)
			ss->Rep_319(*it2);
	}
	ss->Rep_318(sets[1]);
}

void	Frame::printcommand(Session *ss)
{
	std::string res;

	res += "PRIVMSG ";
	res += ss->user().nick();
	res += " :";
	ss->replyAsServer(res + "<Command>");
	ss->replyAsServer(res + "JOIN <channel>[,<channel>] [<key>[,<key>]");
	ss->replyAsServer(res + "PART <channel>[,<channel>]");
	ss->replyAsServer(res + "TOPIC <channel> [<topic>]");
	//ss->replyAsServer("NAMES");
	ss->replyAsServer(res + "LIST [<channel>[,<channel>]]");
	ss->replyAsServer(res + "INVITE <nick> <channel>");
	ss->replyAsServer(res + "KICK <channel> <user> [<comment>]");
}

void	Frame::cmdPong(Session *ss)
{
	ss->setPing(true);
}
