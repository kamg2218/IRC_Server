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

std::string	Executor::split(std::string& buff, std::vector<std::string> & v)
{
	std::string			prefix;
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
	{
		prefix = v[0];
		v.erase(v.begin());
	}
	it = v[0].begin();
	for (; it != v[0].end() ; ++it)
		(*it) = toupper(*it);
	return (prefix);
}

bool	Executor::DoesMatchNick(std::string const& prefix, std::string const& sender_nick)
{
	if (!IsPrefix(prefix))
		return (true);
	if (prefix.substr(1) == sender_nick)
		return (true);
	return (false);
}

void	Executor::execute(std::string& buff, Session* ss)
{
	std::string sender_prefix;
	std::vector<std::string>	splited_cmd;
	Frame *frame;

	frame = Frame().instance();
	sender_prefix = split(buff, splited_cmd);
	if (!DoesMatchNick(sender_prefix, ss->user().nick()))
		return ;
	if (splited_cmd[0] == "NICK")
		return frame->cmdNick(ss, splited_cmd);
	else if (splited_cmd[0] == "USER")
		return frame->cmdUser(ss, splited_cmd);
	else if (splited_cmd[0] == "PASS")
		return frame->cmdPass(ss, splited_cmd);
	if (!(ss->user().IsConnected()))
		ss->Err_451(); //ss->replyAsServer("451");
	else if (splited_cmd[0] == "PRIVMSG")
		frame->cmdPrivmsg(ss, splited_cmd);
	else if (splited_cmd[0] == "QUIT")
		frame->cmdQuit(ss, splited_cmd);
	else if (splited_cmd[0] == "JOIN")
		frame->cmdJoin(ss, splited_cmd);
	else if (splited_cmd[0] == "PART")
		frame->cmdPart(ss, splited_cmd);
	else if (splited_cmd[0] == "KICK")
		frame->cmdKick(ss, splited_cmd);
	else if (splited_cmd[0] == "TOPIC")
		frame->cmdTopic(ss, splited_cmd);
	else if (splited_cmd[0] == "LIST")
		frame->cmdList(ss, splited_cmd);
	else if (splited_cmd[0] == "INVITE")
		frame->cmdInvite(ss, splited_cmd);
	else if (splited_cmd[0] == "WHO")
		frame->cmdWho(ss, splited_cmd);
	else if (splited_cmd[0] == "WHOIS")
		frame->cmdWhois(ss, splited_cmd);
	else if (splited_cmd[0] != "PONG")
		ss->Err_421(splited_cmd[0]); //ss->replyAsServer("421");
}
