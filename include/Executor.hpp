#ifndef FT_EXECUTOR_HPP
# define FT_EXECUTOR_HPP

#include <string>
#include <vector>
#include <map>
#include "User.hpp"

class Executor
{
	private:
		void		split(std::string& buff, std::vector<std::string>& v);
	public:
		bool		gotFullMsg(std::string& buff) const;
		int			msglen(std::string& buff);
		void		reset(std::string& buff);
		bool		execute(std::string& buff, std::map<int, Session*>& ms, Session* ss);
		std::string	Executor::getMessage(std::string& buff)
};

#endif
