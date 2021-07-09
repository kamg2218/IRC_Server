#include <cctype>
#include "include/Executor.hpp"
#include "include/Frame.hpp"
#include "include/User.hpp"

void	Executor::insert(std::string& buff, char *str, int r)
{
	for (int i= 0 ; i < r ; i++)
		buff += str[i];
}

bool	Executor::gotFullMsg(std::string const& buff) const

{
	std::string::size_type res;

	res = buff.find("\r\n");
	if (res == std::string::npos)
		return (false);
	return (true);
}

std::string		Executor::getMessage(std::string& buff)
{
	std::string res;

	res = buff.substr(0, buff.find("\r\n"));
	return (res);
}

void	Executor::reset(std::string& buff)
{
	buff.erase(0, buff.find("\r\n") + 2);
}

int		Executor::msglen(std::string& buff)
{
	std::string::size_type	res;

	res = buff.find("\r\n");
	if (res == std::string::npos)
		return (0);
	return (res);
}

bool	Executor::IsPrefix(std::string const& s)
{
	if (*(s.begin()) == ':')
		return (true);
	return (false);
}

int	Executor::split(std::string& buff, std::vector<std::string> & v)
{
	int					i;
	std::string			msgline;
	std::string::size_type	pos;
	std::string::iterator	it;
	
	msgline = getMessage(buff);
	while ((pos = msgline.find(" ")) != std::string::npos)
	{
		v.push_back(msgline.substr(0, pos));
		msgline.erase(0, msgline.find(" ") + 1);
	}
	if (msgline.size())
		v.push_back(msgline);
	if (IsPrefix(v[0]))
		i = 1;
	else
		i = 0;
	it = v[i].begin();
	for (; it != v[i].end() ; ++it)
	{
		(*it) = toupper(*it);
	}
	return (i);
}


void	Executor::execute(std::string& buff, Session* ss)
{
	int		i;
	std::vector<std::string>	splited_cmd;

	i = split(buff, splited_cmd);
	if (splited_cmd[i] == "NICK")
	{
		// Frame().instance->cmdNick(ss, splited_cmd);
	}
	else if (splited_cmd[i] == "USER")
	{
		// Frame().instance->cmdUser(ss, splited_cmd);
	}
	else if (splited_cmd[i] == "PASS")
	{
		// Frame().instance->cmdPass(ss, splited_cmd);
	}
	// if !registered_user : error 451
	if (splited_cmd[i] == "PRIVMSG")
	{
		// Frame().instance->cmdPrivmsg(ss, splited_cmd);
	}
	else if (splited_cmd[i] == "QUIT")
	{
		// Frame().instance->cmdQuit(ss, splited_cmd);
	}
	else if (splited_cmd[i] == "JOIN")
	{
		// Frame().instance->cmdJoin(ss, splited_cmd);
	}
	else if (splited_cmd[i] == "PART")
	{
		// Frame().instance->cmdPart(ss, splited_cmd);
	}
	else if (splited_cmd[i] == "KICK")
	{
		// Frame().instance->cmdKick(ss, splited_cmd);
	}
	else if (splited_cmd[i] == "TOPIC")
	{
		// Frame().instance->cmdTopic(ss, splited_cmd);
	}
	else if (splited_cmd[i] == "LIST")
	{
		// Frame().instance->cmdList(ss, splited_cmd);
	}
	else if (splited_cmd[i] == "INVITE")
	{
		// Frame().instance->cmdInvite(ss, splited_cmd);
	}
	else if (splited_cmd[i] == "WHO")
	{
		// Frame().instance->cmdWho(ss, splited_cmd);
	}
	else if (splited_cmd[i] == "WHOIS")
	{
		// Frame().instance->cmdWhoi(ss, splited_cmd);
	}
	else if (splited_cmd[i] == "WHOWAS")
	{
		// Frame().instance->cmdWhowas(ss, splited_cmd);
	}
	else
	{
		// Error 421
	}

}
