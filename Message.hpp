#ifndef FT_MESSAGE_HPP
# define FT_MESSAGE_HPP

#include <string>

class Message
{
	std::string buff;
	public:
	
	void operator +=(const char *str)
	{
		buff += str;
	}
	void insert(char *str, int r)
	{
		for (int i= 0 ; i < r ; i++)
			buff += str[i];
	}
	bool gotFullMsg() const
	{
		std::string::size_type res;
		res = buff.find("\r\n");
		if (res == std::string::npos)
			return (false);
		return (true);
	}
	std::string getMessage()
	{
		std::string res = (buff.substr(0,buff.find("\r\n")));
		return (res);
	}
	void reset()
	{
		buff.erase(0, buff.find("\r\n") + 2);
	}
	int msglen()
	{
		std::string res = (buff.substr(0,buff.find("\r\n")));
		return (res.length());
	}

};

#endif
