#ifndef EXECUTOR_HPP
# define EXECUTOR_HPP

#include <string>
#include <vector>
#include <map>
#include "User.hpp"

class Executor
{
	private:
		std::string	split(std::string& buff, std::vector<std::string>& v);
		bool	IsPrefix(std::string const& s);
		bool	DoesMatchNick(std::string const& prefix, std::string const& sender_nick);
	public:
		void	insert(std::string& buff, char *str, int r);
		bool	gotFullMsg(std::string const& buff) const;
		int		msglen(std::string& buff);
		void	reset(std::string& buff);
		void	execute(std::string& buff, Session* ss);

		std::string		getMessage(std::string& buff);
};

#endif
