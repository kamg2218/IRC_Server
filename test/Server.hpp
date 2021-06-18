#ifndef SERVER_HPP
# define SERVER_HPP

#include "Socket.hpp"

class		Server
{
	private:
		Socket		soc;
	public:
		Server();
		Server(int port);
		Server(const Server& other);
		Server&		operator=(const Server& other);
		~Server();
		void		makeServer();
		void		handleAccept();
		class	BindException;
		class	ListenException;
};

#endif
