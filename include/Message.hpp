#ifndef FT_MESSAGE_HPP
# define FT_MESSAGE_HPP

#include <string>

class Message
{
	std::string buff;
	public:
	
	void operator +=(const char *str);
	void insert(char *str, int r);
	bool gotFullMsg() const;
	std::string getMessage();
	int	msglen();
	void reset();
	void cmdNICK();
	void cmdUSER();
};

#endif
