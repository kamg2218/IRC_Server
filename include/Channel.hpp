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
		const std::string	sName;
		std::string 		sTopic;
		std::string			sPassword;
		bool				bPassword;
		Usermap 			mUsers;
		Usermap				mOperators;
	public:
		Channel(Session *creator, std::string const& name, std::string const& topic = "");
		~Channel();
		/*
		void	cmdTopic(std::string const& topic);

		*/
		void	addUser(Session *user);
		void	cmdJoin(Session *ss);
		void	cmdNick(std::string const& name, std::string const& nick);
		void	removeUser(User *user);
		bool	hasUser(User *user);
		void	broadcast(Session *ss, std::string const& message);
		std::string password() const;
		std::string topic() const;
		std::string name() const;
		int		userCount() const;
		bool	hasPass() const;
		bool	isOperator(std::string const& nick) const;
		void	setTopic(std::string topic);
		//void	cmdWho(Session *ss, int check);
		std::vector<std::string> channelVector();
		std::vector<std::string> channeloperVector();
};
#endif
