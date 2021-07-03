#ifndef FT_EXECUTOR_HPP
# define FT_EXECUTOR_HPP

#include <string>
#include <vector>
#include <map>
#include "User.hpp"

class Executor
{
	private:
		//std::string	buff;
		void	split(std::vector<std::string>& v);
	public:
		void	operator +=(const char *str);
		void	insert(char *str, int r);
		bool	gotFullMsg() const;
		int		msglen();
		void	reset();
		void	execute(std::map<int, Session*>& ms, Session* ss);
		std::string		getMessage();
};

#endif
