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
	std::vector<std::string>	chlist;
	std::vector<std::string>	tgtlist;
	std::vector<std::string>	subres;
	std::vector<std::vector<std::string> >	res;
	
	chlist = split_comma(sets[1]);
	tgtlist = split_comma(sets[2]);
	for (int i = 0 ; i < chlist.size() || i < tgtlist.size() ; i++)
	{
		if (i < chlist.size())
			subres.push_back(chlist[i]);
		if (i < tgtlist.size())
			subres.push_back(tgtlist[i]);
		res.push_back(subres);
		subres.clear();
	}
	return (res);
}

void	Frame::cmdKick(Session *ss, std::vector<std::string> const& sets)
{
	std::vector<std::vector<std::string> >	cmdsets;
	std::vector<std::string>	cmd;
	std::vector<std::string>	partcmd;
	Channel *channel;

	if (sets.size() < 3)
	{
		ss->reply("461"); // NOT ENOUGH PARAM
		return ;
	}
	cmdsets = kicklist(sets);
	while(cmdsets.size())
	{
		cmd = cmdsets[0];
		if (cmd.size() < 2)
			ss->reply("461"); //ERR_NEEDMOREPARAMS
		else if (cmd[0].find("#") == std::string::npos && cmd[0].find("&") == std::string::npos)
			ss->reply("461"); //NOT ENOUGH PARAM
		else if (!doesChannelExists(cmd[0].substr(1)))
			ss->reply("403"); //ERR_NOSUCHCHANNEL
		else if (!(ss->user().isMemOfChannel(cmd[0].substr(1))))
			ss->reply("442"); //ERR_NOTONCHANNEL;
		else if (!((channel = findChannel(cmd[0].substr(1)))->isOperator(ss->user().nick())))
			ss->reply("482"); //ERR_CHANOPRIVSNEEDED
		else
		{
			// process
			partcmd.clear();
			partcmd.push_back("PART");
			partcmd.push_back(cmd[0]);
			for (int i = 1 ; i < cmd.size() ; i++)
			{
				partcmd.push_back(cmd[i]);
				cmdPart(ss, partcmd);
				partcmd.pop_back();
			}
		}
		cmdsets.pop_back();
	}
}


void	Frame::cmdInvite(Session *ss, std::vector<std::string> const& sets)
{
	Channel *channel;
	User	*target;
	std::string	rpl;

	if (sets.size() < 3)
	{
		ss->reply("461"); // NOT ENOUGH PARAM
		return ;
	}
	if (sets[1].find("#") == std::string::npos && sets[1].find("&") == std::string::npos)
		ss->reply("461"); //NOT ENOUGH PARAM
	else if (!doesChannelExists(sets[1].substr(1)))
		ss->reply("403"); //NO SUCH CHANNEL
	else if (!(ss->user().isMemOfChannel(sets[1].substr(1))))
		ss->reply("442"); //ERR_NOTONCHANNEL;
	else if (!doesNicknameExists(sets[2]))
		ss->reply("401"); //ERR_NOSUCHNICK
	else if (!((target = findUser(sets[2]))->isMemOfChannel(channel->name())))
		ss->reply("443"); //ERR_USERONCHANNEL;
	else
	{
		// proces
		/*
		target->cmdJoin(mChannels.find(channel->name())); 
		channel->addUser(target);
		channel->broadcast(ss, "");
		*/
		rpl.append(channel->name());
		rpl.append(" ");
		rpl.append(target->nick());
		ss->reply(rpl);
	}
}
