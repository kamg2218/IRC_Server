/*
std::string Frame::cmdKick(Session *ss, std::vector<std::string> const& sets)
{
	Channel	*channel;
	std::vector<std::string>	chlist;
	std::vector<std::string>	tgtlist;
	std::vector<std::string>::const_iterator	it;

	if (sets.size() < 3)
	{
		return ("461"); // ERR_NEEDMOREPARAMS
	}
	for (it = sets.begin() ; it != sets.end() ; ++it)
	{
		if ((*it).find("#") != std::string::npos ||
			(*it).find("&") != std::string::npos)
			chlist.push_back((*it).substr(1));
		else if ((*it) != "KICK")
			break ;
	}
	for (; it != sets.end() ; ++it)
	{
		if ((*it).find(":") != std::string::npos)
			break;
		tgtlist.push_back(*it);
	}
	if (chlist.empty() || tgtlist.empty())
	{
		return ("461"); // ERR_NEEDMOREPARAMS
	}
	for (it = chlist.begin() ; it != chlist.end() ; ++it)
	{
		if (!doesChannelExists(*it))
		{
			return ("403"); //ERR_NOSUCHCHANNEL
		}
		if (!(ss->user().isMemOfChannel((*it))))
		{
			return ("442");	//ERR_NOTONCHANNEL
		}
		channel = findChannel((*it));
		if (!(channel->isOperator(ss->user().nick())))
		{
			return ("482"); //ERR_CHANOPRIVSNEEDED
		}
		// 실행PART
	}
	return "";
}
*/
std::vector<std::vector<std::string> > kicklist(std::vector<std::string> const& sets)
{
	std::vector<std::string> subres;
	std::vector<std::vector<std::string> > res;

	subres.push_back = "KICK";
	while ()
	{
		subres.push_back();
		res.push_back(subres);
		subres.pop_back();
	}
}

std::string Frame::cmdKick(Session *ss, std::vector<std::string> const& sets)
{
	std::vector<std::vector<std::string> >	cmd;

	cmd = kicklist(sets);
	{
	}
}
