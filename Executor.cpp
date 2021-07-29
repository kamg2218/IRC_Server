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
	Frame *frame;

	frame = Frame().instance();
	i = split(buff, splited_cmd);
	if (splited_cmd[i] == "NICK")
	{
		frame->cmdNick(ss, splited_cmd);
		return ;
	}
	else if (splited_cmd[i] == "USER")
	{
		frame->cmdUser(ss, splited_cmd);
		return ;
	}
	else if (splited_cmd[i] == "PASS")
	{
		frame->cmdPass(ss, splited_cmd);
		return ;
	}
	if (!(ss->user().IsConnected()))
	{
		ss->replyAsServer("451");
	}
	else if (i == 1)
	{
		return ;
	}
	else if (splited_cmd[i] == "PRIVMSG")
	{
		//frame->cmdPrivmsg(ss, splited_cmd);
	}
	else if (splited_cmd[i] == "QUIT")
	{
		frame->cmdQuit(ss, splited_cmd);
	}
	else if (splited_cmd[i] == "JOIN")
	{
		frame->cmdJoin(ss, splited_cmd);
	}
	else if (splited_cmd[i] == "PART")
	{
		frame->cmdPart(ss, splited_cmd);
	}
	else if (splited_cmd[i] == "KICK")
	{
		frame->cmdKick(ss, splited_cmd);
	}
	else if (splited_cmd[i] == "TOPIC")
	{
		frame->cmdTopic(ss, splited_cmd);
	}
	else if (splited_cmd[i] == "LIST")
	{
		frame->cmdList(ss, splited_cmd);
	}
	else if (splited_cmd[i] == "INVITE")
	{
		frame->cmdInvite(ss, splited_cmd);
	}
	else if (splited_cmd[i] == "WHO")
	{
		// frame->cmdWho(ss, splited_cmd);
	}
	else if (splited_cmd[i] == "WHOIS")
	{
		// frame->cmdWhoi(ss, splited_cmd);
	}
	else if (splited_cmd[i] == "WHOWAS")
	{
		// frame->cmdWhowas(ss, splited_cmd);
	}
	else if (splited_cmd[i] != "PONG")
	{
		// Error 421
		ss->replyAsServer("421");
	}

}
