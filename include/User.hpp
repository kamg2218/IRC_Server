#ifndef USER_HPP
# define USER_HPP

#include <string>
#include <vector>
#include <list>
class Session;
#include "Channel.hpp"

typedef std::map<std::string, Channel*>  ChannelMap;

class User
{
	private:
		std::string		_sRealname;
		std::string		_sNickname;
		std::string		_sHostname;
		std::string		_sUsername;
		ChannelMap		_mChannels;
		bool			_didNick;
		bool			_didUser;
		bool			_manager;
		bool			_password;
		bool			_isProperlyQuit;
		std::list<std::string>	_pastNick;
		User(User const& other);
		User&		operator=(User const& other);
		std::string		makeLower(std::string const& str);
	public:
		User(void);
		~User(void);
		void		cmdWhois(Session *ss);
		void		cmdJoin(Channel* ch);
		void		cmdNick(std::vector<std::string> const& sets);
		void		optionJoin(Session *ss, std::string const& msg);
		void		cmdQuit(void);
		void		cmdOper(void);
		void		setPass(bool pass);
		void		partChannel(Channel *ch);
		void		setProperlyQuit(bool state);
		void		setName(std::string const& s);
		void		setNick(std::string const& s);
		void		setHost(std::string const& s);
		void		setUser(std::string const& s);
		bool		pass(void) const;
		bool		checkNick(void) const;
		bool		checkUser(void) const;
		bool		checkManager(void) const;
		bool		checkQuit(void) const;
		bool		isRegistered(void) const;
		bool		addNick(std::vector<std::string> const& sets);
		bool		cmdUser(std::vector<std::string> const& sets);
		bool		isMemOfChannel(std::string const& chname) const;
		bool		cmdPart(Session *ss, std::string const& chname, std::vector<std::string> const& sets);
		std::string		name(void) const;
		std::string		nick(void) const;
		std::string		host(void) const;
		std::string		user(void) const;
		std::string		msgHeader(void) const;
		std::vector<std::string>	userVectorOper(std::vector<std::string> const& sets);
};

#endif
