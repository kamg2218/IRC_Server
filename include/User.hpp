#ifndef USER_HPP
# define USER_HPP

#include <string>
#include <vector>
#include <list>
class Session;
#include "Socket.hpp"
class User;
#include "Channel.hpp"

typedef std::map<std::string, Channel*>  ChannelMap;

class User
{
	private:
		std::string		sRealname;
		std::string		sNickname;
		std::string		sHostname;
		std::string		sUsername;
		ChannelMap		mChannels;
		bool	didNick;
		bool	didUser;
		bool	manager;
		bool	is_properly_quit;
		std::list<std::string>	_pastNick;
	public:
		User();
		~User();
		static User*	create();
		bool	addNick(std::vector<std::string> const& sets);
		void	cmdNick(std::vector<std::string> const& sets);
		bool	cmdUser(std::vector<std::string> const& sets);
		void	cmdJoin(Channel* ch);
		void	optionJoin(Session *ss, std::vector<std::string> const& sets, std::string const& msg);
		bool	cmdPart(Session *ss, std::string const& sets, std::string const& msg);
		void	cmdQuit(Session *ss, std::vector<std::string> const& sets, std::string const& msg);
		void		setName(std::string const& s);
		void		setNick(std::string const& s);
		void		setHost(std::string const& s);
		void		setUser(std::string const& s);
		std::string	name() const;
		std::string nick() const;
		std::string host() const;
		std::string user() const;
		std::string	msgHeader() const;
		bool	CheckNick() const;
		bool	CheckUser() const;
		bool	CheckManager() const;
		bool	IsConnected() const;
		void	cmdOper();
		bool	isMemOfChannel(std::string const& chname) const;
		const ChannelMap&	channel() const;	// tmp
		std::vector<std::string> userVector();
		std::vector<std::string> cmdWhois();
		//std::string cmdWhowas(std::string nickname, int num);
};

#endif
