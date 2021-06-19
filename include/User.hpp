#ifndef FT_USER_HPP
# define FT_USER_HPP

#include <string>

class Session;
class User;
#include "Channel.hpp"

typedef std::map<std::string, Channel*>  Channelmap;

class User
{
	private:
		Session			*mysession;
		std::string		sRealname;
		std::string		sNickname;
		std::string		sHost;
		Channelmap		mChannels;
		bool	didNick;
		bool	didUser;
		bool	is_properly_quit;
	public:
		User(Session* ms);
		~User();
		/*
		void	cmdNick();
		void	cmdUser();
		void	cmdJoin();
		void	cmdKick();
		void	cmdPart();
		void	cmdQuit();
		*/
		void		setName(std::string const& s);
		void		setNick(std::string const& s);
		void		setHost(std::string const& s);
		std::string	name() const;
		std::string nick() const;
		std::string Host() const;
		std::string	msgHeader() const;
};
#endif
