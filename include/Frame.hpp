#ifndef FRAME_HPP
# define FRAME_HPP

#include <string>
#include <map>
#include <ctime>
#include <cctype>
#include "User.hpp"
#include "Channel.hpp"
#include "MainServer.hpp"
#include "Service.hpp"
#include <set>

typedef std::map<std::string, Session*> UserMap;
typedef std::map<std::string, Channel*> ChannelMap;

class Frame
{
	private:
		static Frame*	_pInstance;
		UserMap			_mUsers;
		ChannelMap		_mChannels;
		MainServer		_server;
		Service			_service;
		Frame(Frame const& other);
		Frame&	operator=(Frame const& other);
		bool			checkMask(std::string const& str, std::string const& name, int wild);
		void			printCommand(Session *ss);
		void			broadcastAll(Session *ss, std::string const& str);
		void			doJoin(Session *ss, std::vector<std::string> const& sets, std::string const& chname);
		std::string		makeLower(std::string const& str);
		std::string		vectorToString(std::vector<std::string> const& sets);
		std::vector<std::string>	userMask(std::string const& str);
		std::vector<std::string>	channelMask(std::string const& str);
		std::vector<std::string>	split_comma(std::string s);
		std::vector<std::vector<std::string> >		kicklist(std::vector<std::string> const& sets);
	public:
		Frame(void);
		~Frame(void);
		static Frame*		instance(void);
		void		start(Base const& bs);
		void		addChannel(Channel* new_chan);
		void		removeChannel(std::string const& name);
		void		removeUser(std::string const& nick);
		void		removeAllUser(void);
		void		removeAllChannel(void);
		bool		addUser(Session *new_user);
		bool		checkNickname(std::string const& name);
		bool		checkChannelname(std::string const& name);
		bool		doesChannelExists(std::string const& name);
		bool		doesNicknameExists(std::string const& name);
		MainServer&	getServer(void);
		Session*	findUser(std::string const& name);
		Channel*	findChannel(std::string const& name);
		void		cmdPong(Session *ss);
		void		cmdPart(Session *ss, std::vector<std::string> const& sets);
		void		cmdQuit(Session *ss, std::vector<std::string> const& sets);
		void		cmdUser(Session *ss, std::vector<std::string> const& sets);
		void		cmdJoin(Session *ss, std::vector<std::string> const& sets);
		void		cmdNick(Session *ss, std::vector<std::string> const& sets);
		void		cmdPass(Session *ss, std::vector<std::string> const& sets);
		void		cmdTopic(Session *ss, std::vector<std::string> const& sets);
		void		cmdList(Session *ss, std::vector<std::string> const& sets);
		void		cmdKick(Session *ss, std::vector<std::string> const& sets);
		void		cmdInvite(Session *ss, std::vector<std::string> const& sets);
		void		cmdWho(Session *ss, std::vector<std::string> const& sets);
		void		cmdWhois(Session *ss, std::vector<std::string> const& sets);
		void		cmdPrivmsg(Session *ss, std::vector<std::string> const& sets);
};

#endif
