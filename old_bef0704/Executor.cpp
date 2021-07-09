#include <cctype>
#include "include/Executor.hpp"
#include "include/Frame.hpp"
#include "include/User.hpp"

void	Executor::operator +=(const char *str)
{
	buff += str;
}

void	Executor::insert(char *str, int r)
{
	for (int i= 0 ; i < r ; i++)
		buff += str[i];
}

bool	Executor::gotFullMsg() const
{
	std::string::size_type res;
	res = buff.find("\r\n");
	if (res == std::string::npos)
		return (false);
	return (true);
}

std::string		Executor::getMessage()
{
	std::string res = (buff.substr(0,buff.find("\r\n")));
	return (res);
}

void	Executor::reset()
{
	buff.erase(0, buff.find("\r\n") + 2);
}

int		Executor::msglen()
{
	std::string res = (buff.substr(0,buff.find("\r\n")));
	return (res.length());
}

void	Executor::split(std::vector<std::string> & v)
{
	std::string msgline	= getMessage();
	std::string::size_type	pos;
	
	while ((pos = msgline.find(" ")) != std::string::npos)
	{
		v.push_back(msgline.substr(0, pos));
		msgline.erase(0, msgline.find(" ") + 1);
	}
	if (msgline.size())
		v.push_back(msgline);
	std::string::iterator it = v[0].begin();
	for (; it != v[0].end() ; ++it)
	{
		*it = toupper(*it);
	}
}

void	Executor::execute(User *user)
{
	std::vector<std::string>	splited_cmd;

	split(splited_cmd);
	if (splited_cmd[0] == "NICK")
	{
		/*
		user->cmdNick();
		user->session->sendAsServer();
		*/
	}
	else if (splited_cmd[0] == "USER")
	{
		/*
		user->cmdUser();
		*/
	}
	/*
	// print splited cmd vector
	std::cout << "CMD START--------\n";
	while (it != splited_cmd.end())
	{
		std::cout << (*it) << std::endl;
		++it;
	}
	std::cout << "CMD END----------\n";
	*/
}
