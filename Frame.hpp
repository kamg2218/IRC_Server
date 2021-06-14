#ifndef FT_FRAME_HPP
# define FT_FRAME_HPP

#include <string>
#include <map>
#include "User.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "Socket.hpp"

typedef std::map<std::string, User*> UserMap;
typedef std::map<std::string, Channel*> ChannelMap;
class Frame
{
	private:
		static Frame* pInstance;
		UserMap		mUsers;
		ChannelMap	mChannels;
		Socket		socket;
		Server		server;
	public:
		~Frame()
		{
			removeAllChannel();
		}
		static Frame* instance()
		{
			if (!pInstance)
				pInstance = new Frame();
			return (pInstance);
		} 
		void start()
		{
			server.create(80);
			socket.addServer(server.getSockfd(), &server);
			while (1)
			{
				socket.do_select();
				do_service();
			}
		}
		void do_service()
		{
			/*
			int r ;
			r = socket.getRes();
			m = socket.getMaxopen();
			for (int i = 0 ; i < m && r > 0 ; i++)
			{
				if (socket.check_read(i))
					fds[i].read();
				if (socket.check_write(i))
					fds[i].write();
				if (socket.check_read(i) || socket.check_write(i))
					r--;
			}
			*/
			std::cout << "hi\n";
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
