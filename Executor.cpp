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

void	Executor::execute()
{
	std::cout << "cmd is executing\n";
}
