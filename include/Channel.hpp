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
		int			userCount() const;
		int			operCount() const;
		void		addUser(Session *user);
		void		addOper(Session *user);
		void		setTopic(std::string topic);
		void		removeUser(std::string const& nick);
		void		removeOper(std::string const& nick);
		void		cmdJoin(Session *ss);
		void		cmdPart(std::string const& nick);
		void		broadcast(Session *ss, std::string const& message);
		void		cmdNick(std::string const& name, std::string const& nick);
		void		privmsgBroadcast(Session *ss, std::string const& message);
		void		cmdTopic(Session *ss, std::string const& topic, std::string const& msg);
		bool		hasPass() const;
		bool		hasUser(std::string const& nick) const;
		bool		hasOper(std::string const& nick) const;
		std::string name() const;
		std::string topic() const;
		std::string password() const;
		std::vector<std::string>	channelVector();
		std::vector<std::string>	channeloperVector();
};

#endif
