#ifndef FT_FRAME_HPP
# define FT_FRAME_HPP


#include <string>
#include <map>
#include "User.hpp"
#include "Channel.hpp"
#include "MainServer.hpp"
#include "Server.hpp"
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
		void	removeUser(std::string & nick);
		bool	doesChannelExists(std::string const& name);
		void	addChannel(Channel* new_chan);
		void	removeChannel(std::string const& name);
		void	removeAllChannel();
		ChannelMap::iterator	findChannel(std::string const& name);
		void	cmdPart(Session *ss, std::vector<std::string> const& sets);
		void	cmdQuit(Session *ss, std::vector<std::string> const& sets);
};

#endif
