#include "include/Message.hpp"
#include "include/Frame.hpp"

void	Message::operator +=(const char *str)
{
	buff += str;
}

void	Message::insert(char *str, int r)
{
	for (int i= 0 ; i < r ; i++)
		buff += str[i];
}

bool	Message::gotFullMsg() const
{
	std::string::size_type res;
	res = buff.find("\r\n");
	if (res == std::string::npos)
		return (false);
	return (true);
}

std::string		Message::getMessage()
{
	std::string res = (buff.substr(0,buff.find("\r\n")));
	return (res);
}

void	Message::reset()
{
	buff.erase(0, buff.find("\r\n") + 2);
}

int		Message::msglen()
{
	std::string res = (buff.substr(0,buff.find("\r\n")));
	return (res.length());
}

void	Message::cmdNICK()
{
	Frame *mainframe = Frame::instance();
	
	//mainframe.addUser();
}

void	Message::cmdUSER()
{
}
