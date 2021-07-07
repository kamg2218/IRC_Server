#include <cctype>
#include "include/Executor.hpp"
#include "include/Frame.hpp"
#include "include/User.hpp"

/*
void	Executor::operator +=(std::string& buff, const char *str)
{
	buff += str;
}*/

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

bool	Executor::split(std::string& buff, std::vector<std::string> & v)
{
	int					i;
	bool				res;
	std::string msgline	= getMessage(buff);
	std::string::size_type	pos;
	std::string::iterator	it;
	
	while ((pos = msgline.find(" ")) != std::string::npos)
	{
		v.push_back(msgline.substr(0, pos));
		msgline.erase(0, msgline.find(" ") + 1);
	}
	if (msgline.size())
		v.push_back(msgline);
	if ((res = IsPrefix(v[0])))
		i = 1;
	else
		i = 0;
	it = v[i].begin();
	for (; it != v[i].end() ; ++it)
	{
		(*it) = toupper(*it);
	}
	return (res);
}

void	Executor::execute(std::string& buff, std::map<int, Session*>& ms, Session* ss)
{
	bool	prefixed;
	int		i;
	std::vector<std::string>	splited_cmd;

	prefixed = split(buff, splited_cmd);
	if (prefixed)
		i = 1;
	else
		i = 0;
	std::cout << "CMD START--------\n";
	if (prefixed)
		std::cout << "It has prefix\n";
	else
		std::cout << "No prefix\n";
	std::vector<std::string>::iterator it = splited_cmd.begin();
	while (it != splited_cmd.end())
	{
		std::cout << (*it) << std::endl;
		++it;
	}
	std::cout << "CMD END----------\n";

	if (splited_cmd[i] == "NICK")
	{
		std::cout << "NICK is called\n";
		/*
		user->cmdNick(splited_cmd);
		user->session->sendAsServer();
		*/
	}
	else if (splited_cmd[i] == "USER")
	{
		std::cout << "USER is called\n";
		/*
		user->cmdUser(splited_cmd);
		*/
	}
	else if (splited_cmd[i] == "PASS")
	{
	}
	// if !registered_user : error 451

	if (splited_cmd[i] == "QUIT")
	{
		std::cout << "QUIT is called\n";
		/*
		user->cmdQUIT(splited_cmd);
		*/
	}

	// print splited cmd vector
}

void	Reply(std::stringg const& s)
{
}
