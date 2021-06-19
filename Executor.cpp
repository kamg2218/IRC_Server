#include "include/Executor.hpp"
#include "include/Frame.hpp"

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
	std::string msgline = getMessage();
	std::string::size_type	pos;
	
	std::cout << "FULL : \n" << msgline << "\n";
	while ((pos = msgline.find(" ")) != std::string::npos)
	{
		v.push_back(msgline.substr(0, pos));
		msgline.erase(0, msgline.find(" ") + 1);
	}
	if (msgline.size())
		v.push_back(msgline);
}

void	Executor::execute()
{
	std::vector<std::string> splited_cmd;

	split(splited_cmd);


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
