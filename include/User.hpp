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
	public:
		User(void);
		User(User const& other);
		User&		operator=(User const& other);
		~User(void);
		static User*		create(void);
		bool		addNick(std::vector<std::string> const& sets);
		void		cmdNick(std::vector<std::string> const& sets);
		bool		cmdUser(std::vector<std::string> const& sets);
		void		cmdJoin(Channel* ch);
		void		optionJoin(Session *ss, std::vector<std::string> const& sets, std::string const& msg);
		bool		cmdPart(Session *ss, std::string const& chname, std::vector<std::string> const& sets);
		void		cmdQuit(Session *ss, std::vector<std::string> const& sets, std::string const& msg);
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
		bool		isConnected(void) const;
		bool		isMemOfChannel(std::string const& chname) const;
		std::string		name(void) const;
		std::string		nick(void) const;
		std::string		host(void) const;
		std::string		user(void) const;
		std::string		msgHeader(void) const;
		std::string		makeLower(std::string const& str);
		const ChannelMap&		channel(void) const;	// tmp
		std::vector<std::string>		userVector(void);
		std::vector<std::string>		cmdWhois(void);
		//std::string cmdWhowas(std::string nickname, int num);
};

#endif
