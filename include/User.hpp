#ifndef FT_USER_HPP
# define FT_USER_HPP

#include <string>
#include <vector>
#include <list>
class Session;
class User;
#include "Socket.hpp"
#include "Channel.hpp"

typedef std::map<std::string, Channel*>  ChannelMap;

class User
{
	private:
		Session			*mysession;
		std::string		sRealname;
		std::string		sNickname;
		std::string		sHost;
		ChannelMap		mChannels;
		bool	didNick;
		bool	didUser;
		bool	is_properly_quit;
		std::list<std::string>	_pastNick;
	public:
		User(Session *ms);
		~User();
		static User*	create(Session *ms);
		bool	addNick(std::vector<std::string> const& sets);
		void	cmdNick(std::vector<std::string> const& sets);
		bool	cmdUser();
		void	cmdJoin(std::pair<std::string, Channel*> const& it);
		void	cmdKick(std::vector<std::string> const& sets);
		void	cmdPart(std::vector<std::string> const& sets);
		void	cmdQuit(std::vector<std::string> const& sets);
		void		setName(std::string const& s);
		void		setNick(std::string const& s);
		void		setHost(std::string const& s);
		std::string	name() const;
		std::string nick() const;
		std::string Host() const;
		std::string	msgHeader() const;
		bool	CheckNick() const;
		bool	CheckUser() const;
};

#endif
