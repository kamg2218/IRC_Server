#ifndef EXECUTOR_HPP
# define EXECUTOR_HPP

#include <string>
#include <vector>
#include <map>
#include "User.hpp"

class Executor
{
	private:
		Executor(Executor const& ref);
		Executor&		operator=(Executor const& ref);
		std::string		split(std::string& buff, std::vector<std::string>& v);
		bool		isPrefix(std::string const& s) const;
		bool		doesMatchNick(std::string const& prefix, std::string const& sender_nick) const;
	public:
		Executor();
		~Executor();
		std::string		getMessage(std::string const& buff) const;
		void		insert(std::string& buff, char *str, int r);
		bool		gotFullMsg(std::string const& buff) const;
		int			msglen(std::string& buff) const;
		void		reset(std::string& buff);
		void		execute(std::string& buff, Session* ss);
};

#endif
