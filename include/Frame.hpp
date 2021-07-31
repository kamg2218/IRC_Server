#ifndef FRAME_HPP
# define FRAME_HPP

#include <string>
#include <map>
#include <cctype>
#include "User.hpp"
#include "Channel.hpp"
#include "MainServer.hpp"
#include "Service.hpp"

typedef std::map<std::string, Session*> UserMap;
typedef std::map<std::string, Channel*> ChannelMap;
/*
typedef struct base
{
    std::string host;
    int server_port;
    std::string password_network;
    int port;
    std::string password;
  
} base;
*/
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
		void	start(base const& bs);
		bool	doesNicknameExists(std::string const& name);
		bool	addUser(Session *new_user);
		void	removeUser(std::string const& nick);
		bool	doesChannelExists(std::string const& name);
		void	addChannel(Channel* new_chan);
		void	removeChannel(std::string const& name);
		void	removeAllChannel();
		bool	CheckNickname(std::string const& name);
		bool	CheckChannelname(std::string const& name);
		std::string	MakeLower(std::string const& str);
		Session*	findUser(std::string const& name);
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
		void	doPart(Session *ss, std::string const& sets, std::string const& re);
		void	doJoin(Session *ss, std::string const& sets, std::string const& msg);
		void	doList(Session *ss, std::string const& sets);
		void	cmdKick(Session *ss, std::vector<std::string> const& sets);
		void	cmdInvite(Session *ss, std::vector<std::string> const& sets);
		bool	checkMask(std::string const& str, std::string const& name, int wild);
		std::vector<std::string>	getMask(std::string const& str);
		void	cmdWho(Session *ss, std::vector<std::string> const& sets);
		void	cmdWhois(Session *ss, std::vector<std::string> const& sets);
		void	cmdWhowas(Session *ss, std::vector<std::string> const& sets);
		void	cmdPrivmsg(Session *ss, std::vector<std::string> const& sets);
		std::string	vectorToString(std::vector<std::string> const& sets);
		MainServer& GetServer(void);

};

#endif


