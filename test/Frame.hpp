#ifndef FRAME_HPP
# define FRAME_HPP

#include <map>
#include "Socket.hpp"
#include "Server.hpp"
#include "Service.hpp"

class	Frame 
{
	private:
		Server	sev;
		Service	svi;
		std::map<int, Socket>	mSockets;
	public:
		Frame();
		Frame(int port);
		Frame(const Frame& other);
		void	operator=(const Frame& other);
		~Frame();
		void	start();
};

#endif
