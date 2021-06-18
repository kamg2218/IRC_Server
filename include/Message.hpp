#ifndef FT_MESSAGE_HPP
# define FT_MESSAGE_HPP

#include <string>

class Message
{
	private:
		std::string	buff;

	public:
		void	operator +=(const char *str);
		void	insert(char *str, int r);
		bool	gotFullMsg() const;
		int		msglen();
		void	reset();
		void	cmdNICK();
		void	cmdUSER();
		std::string		getMessage();
};

#endif
