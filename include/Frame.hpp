#ifndef FRAME_HPP
# define FRAME_HPP

#include <string>
#include <map>
#include <cctype>
#include "User.hpp"
#include "Channel.hpp"
#include "MainServer.hpp"
#include "Service.hpp"

typedef std::map<std::string, User*> UserMap;
typedef std::map<std::string, Channel*> ChannelMap;

class Frame
{
	private:
		static Frame*	pInstance;
		UserMap			mUsers;
		ChannelMap		mChannels;
		MainServer		server;
		Service			service;
	public:
		~Frame();
		static Frame*	instance();
		void	start();
		bool	doesNicknameExists(std::string const& name);
		bool	addUser(User *new_user);
		void	removeUser(std::string const& nick);
		bool	doesChannelExists(std::string const& name);
		void	addChannel(Channel* new_chan);
		void	removeChannel(std::string const& name);
		void	removeAllChannel();
		bool	CheckNickname(std::string const& name);
		bool	CheckChannelname(std::string const& name);
		std::string	MakeLower(std::string const& str);
		Channel*	findChannel(std::string const& name);
		void	cmdPart(Session *ss, std::vector<std::string> const& sets);
		void	cmdQuit(Session *ss, std::vector<std::string> const& sets);
		void	cmdUser(Session *ss, std::vector<std::string> const& sets);
		void	cmdJoin(Session *ss, std::vector<std::string> const& sets);
		void	cmdNick(Session *ss, std::vector<std::string> const& sets);
		void	cmdPass(Session *ss, std::vector<std::string> const& sets);
		void	cmdOper(Session *ss, std::vector<std::string> const& sets);
		void	cmdTopic(Session *ss, std::vector<std::string> const& sets);
		void	cmdList(Session *ss, std::vector<std::string> const& sets);
		std::string	doPart(Session *ss, std::string const& sets);
		std::string	doJoin(Session *ss, std::string const& sets);
		std::string	doList(Session *ss, std::string const& sets);
		User*	findUser(std::string const& name);
		void	cmdKick(Session *ss, std::vector<std::string> const& sets);
};

#endif
