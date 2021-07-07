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
		void	split(std::string& buff, std::vector<std::string>& v);
	public:
		//void	operator +=(std::string& buff, const char *str);
		void	insert(std::string& buff, char *str, int r);
		bool	gotFullMsg(std::string& buff) const;
		int		msglen(std::string& buff);
		void	reset(std::string& buff);
		void	execute(std::string& buff, std::map<int, Session*>& ms, Session* ss);
		std::string		getMessage(std::string& buff);
};

#endif
