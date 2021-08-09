#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <map>
class Channel;
#include "User.hpp"
typedef std::map<std::string, Session*> Usermap;

class Channel
{
	private:
		const std::string		_sName;
		std::string		_sTopic;
		Usermap		_mUsers;
		Usermap		_mOperators;
	public:
		Channel(Session *creator, std::string const& name, std::string const& topic = "");
		~Channel();
		void		addUser(Session *user);
		void		addOper(Session *user);
		void		cmdJoin(Session *ss);
		void		cmdTopic(std::string const& topic, std::string const& msg);
		void		cmdNick(std::string const& name, std::string const& nick);
		void		removeUser(User *user);
		bool		hasUser(User *user);
		void		broadcast(Session *ss, std::string const& message);
		void		privmsgBroadcast(Session *ss, std::string const& message);
		std::string 	password() const;
		std::string 	topic() const;
		std::string 	name() const;
		int		userCount() const;
		bool		hasPass() const;
		bool		isOperator(std::string const& nick) const;
		void		setTopic(std::string topic);
		std::vector<std::string>		channelVector();
		std::vector<std::string>		channeloperVector();
};
#endif
