#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <map>
class Channel;
#include "User.hpp"
typedef std::map<std::string, Session*> UserMap;

class Channel
{
	private:
		const std::string	_sName;
		std::string			_sTopic;
		UserMap				_mUsers;
		UserMap				_mOperators;
	public:
		Channel(Session *creator, std::string const& name, std::string const& topic = "");
		Channel(Channel const& other);
		Channel&	operator=(Channel const& other);
		~Channel(void);
		void		addUser(Session *user);
		void		addOper(Session *user);
		void		cmdJoin(Session *ss);
		void		cmdPart(std::string const& nick);
		void		cmdTopic(Session *ss, std::string const& topic, std::string const& msg);
		void		cmdNick(std::string const& name, std::string const& nick);
		void		removeUser(std::string const& nick);
		void		removeOper(std::string const& nick);
		bool		hasUser(std::string const& nick);
		bool		hasOper(std::string const& nick);
		void		broadcast(Session *ss, std::string const& message);
		void		privmsgBroadcast(Session *ss, std::string const& message);
		std::string password() const;
		std::string topic() const;
		std::string name() const;
		int			userCount() const;
		int			operCount() const;
		bool		hasPass() const;
		bool		isOperator(std::string const& nick) const;
		void		setTopic(std::string topic);
		std::vector<std::string>	channelVector();
		std::vector<std::string>	channeloperVector();
};

#endif
