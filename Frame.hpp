#ifndef FT_FRAME_HPP
# define FT_FRAME_HPP

#include <string>
#include <map>
#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Service.hpp"
#include "Session.hpp"

typedef std::map<std::string, User*> UserMap;
typedef std::map<std::string, Channel*> ChannelMap;
class Frame
{
	private:
		static Frame* pInstance;
		UserMap		mUsers;
		ChannelMap	mChannels;
		Server		server;
		Service		service;
		std::map<int, Session*> mSessions;
	public:
		~Frame()
		{
			removeAllChannel();
		}
		static Frame* instance()
		{
			if (!pInstance)
			{
				pInstance = new Frame();
			}
			return (pInstance);
		} 
		void	start()
		{
			server.create(80);
			while(1)
			{
				service.do_select(server, mSessions);
				service.do_service(server, mSessions);
			}

		}
		bool doesNicknameExists(std::string const& name)
		{
			return (mUsers.find(name) != mUsers.end());
		}

		bool addUser(User *new_user)
		{
			if (doesNicknameExists(new_user->nick()))
				return (false);
			mUsers[new_user->nick()] = new_user;
			return (true);
		}
		void removeUser(std::string & nick)
		{
			mUsers.erase(nick);
		}
		bool doesChannelExists(std::string const& name)
		{
			return (mChannels.find(name) != mChannels.end());
		}
		void addChannel(Channel* new_chan)
		{
			if (!doesChannelExists(new_chan->name()))
				mChannels[new_chan->name()] = new_chan;
		}
		void removeChannel(std::string const& name)
		{
			mChannels.erase(name);
		}
		void removeAllChannel()
		{
			ChannelMap::iterator it;

			for (it = mChannels.begin() ; it != mChannels.end() ; ++it)
			{
				delete (it->second);
			}
		}
};

Frame * Frame::pInstance = NULL;

#endif
