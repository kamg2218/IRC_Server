#include "include/Executor.hpp"
#include "include/Frame.hpp"
#include "include/Session.hpp"

Executor::Executor(void)
{
}

Executor::~Executor(void)
{
}

Executor::Executor(Executor const& ref)
{
	*this = ref;
}

Executor&		Executor::operator=(Executor const& ref)
{
	if (this == &ref)
		return *this;
	return *this;
}

bool		Executor::gotFullMsg(std::string const& buff) const

{
	std::string::size_type		res;

	res = buff.find(RNDELI);
	if (res != std::string::npos)
		return true;
	res = buff.find(NDELI);
	if (res != std::string::npos)
		return true;
	return false;
}

std::string		Executor::getMessage(std::string const& buff) const
{
	if (buff.find(RNDELI) != std::string::npos)
		return buff.substr(0, buff.find(RNDELI));
	if (buff.find(NDELI) != std::string::npos)
		return buff.substr(0, buff.find(NDELI));
	return "";
}

void		Executor::reset(std::string& buff)
{
	if (buff.find(RNDELI) != std::string::npos)
		buff.erase(0, buff.find(RNDELI) + 2);
	else if (buff.find(NDELI) != std::string::npos)
		buff.erase(0, buff.find(NDELI) + 1);
}

int		Executor::msglen(std::string& buff) const
{
	std::string::size_type		res;

	res = buff.find(RNDELI);
	if (res != std::string::npos)
		return res;
	res = buff.find(NDELI);
	if (res != std::string::npos)
		return res;
	return 0;
}

bool		Executor::isPrefix(std::string const& s) const
{
	if (s.size() && *(s.begin()) == ':')
		return true;
	return false;
}

std::string		Executor::split(std::string& buff, std::vector<std::string> & v)
{
	std::string		prefix;
	std::string		msgline;
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
	if (!v.size())
		return prefix;
	if (isPrefix(v[0]))
	{
		prefix = v[0];
		v.erase(v.begin());
	}
	it = v[0].begin();
	for (; it != v[0].end() ; ++it)
		(*it) = std::toupper(*it);
	return prefix;
}

bool		Executor::doesMatchNick(std::string const& prefix, std::string const& sender_nick) const
{
	if (!isPrefix(prefix))
		return true;
	if (prefix.substr(1) == sender_nick)
		return true;
	return false;
}

void		Executor::execute(std::string& buff, Session* ss)
{
	Frame			*frame;
	std::string		sender_prefix;
	std::vector<std::string>	splited_cmd;
	
	frame = Frame::instance();
	sender_prefix = split(buff, splited_cmd);
	if (!doesMatchNick(sender_prefix, ss->user().nick()))
		return ;
	if (splited_cmd[0] == "NICK")
		return frame->cmdNick(ss, splited_cmd);
	else if (splited_cmd[0] == "USER")
		return frame->cmdUser(ss, splited_cmd);
	else if (splited_cmd[0] == "PASS")
		return frame->cmdPass(ss, splited_cmd);
	else if (splited_cmd[0] == "PONG")
		return frame->cmdPong(ss);
	if (!(ss->user().isRegistered()))
		ss->err451();
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
	else
		ss->err421(splited_cmd[0]);
}
