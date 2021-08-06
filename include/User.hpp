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
		User();
		User(const User& other);
		User&		operator=(const User& other);
		~User();
		static User*		create();
		bool		addNick(std::vector<std::string> const& sets);
		void		cmdNick(std::vector<std::string> const& sets);
		bool		cmdUser(std::vector<std::string> const& sets);
		void		cmdJoin(Channel* ch);
		void		optionJoin(Session *ss, std::vector<std::string> const& sets, std::string const& msg);
		bool		cmdPart(Session *ss, std::string const& chname, std::vector<std::string> const& sets);
		void		cmdQuit(Session *ss, std::vector<std::string> const& sets, std::string const& msg);
		void		cmdOper();
		void		setPass(bool pass);
		void		partChannel(Channel *ch);
		void		setProperlyQuit(bool state);
		void		setName(std::string const& s);
		void		setNick(std::string const& s);
		void		setHost(std::string const& s);
		void		setUser(std::string const& s);
		bool		pass() const;
		bool		checkNick() const;
		bool		checkUser() const;
		bool		checkManager() const;
		bool		isConnected() const;
		bool		isMemOfChannel(std::string const& chname) const;
		std::string		name() const;
		std::string		nick() const;
		std::string		host() const;
		std::string		user() const;
		std::string		msgHeader() const;
		std::string		makeLower(std::string const& str);
		const ChannelMap&		channel() const;	// tmp
		std::vector<std::string>		userVector();
		std::vector<std::string>		cmdWhois();
		//std::string cmdWhowas(std::string nickname, int num);
};

#endif
