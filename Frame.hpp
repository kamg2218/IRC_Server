#ifndef FRAME_HPP
# define FRAME_HPP

#include <map>
#include "User.hpp"
#include "Socket.hpp"
#include "Server.hpp"
#include "Service.hpp"

class	Frame 
{
	private:
		Server	sev;
		Service	svi;
		std::map<int, User>	mSockets;
	public:
		Frame();
		Frame(int port);
		Frame(const Frame& other);
		Frame&	operator=(const Frame& other);
		~Frame();
		void	start();
};

#endif
