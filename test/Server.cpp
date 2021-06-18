#include "Server.hpp"

Server::Server()
	: max_fd(0)
{
	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	FD_ZERO(&except);
	tv.tv_sec = 0;
	tv.tv_usec = 0;
}

Server::Server(int s)
	: max_fd(0)
{
	max_fd = s;
	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	FD_ZERO(&except);
	FD_SET(s, &rfds);
	FD_SET(s, &wfds);
	FD_SET(s, &except);
	tv.tv_sec = 5;
	tv.tv_usec = 0;
}

Server::Server(const Server& other)
{
	return *this = other;
}

Server&		Server::operator=(const Server& other)
{
	if (this == &other)
		return *this;
	max_fd = other.max_fd;
	tv = other.tv;
	rfds = other.rfds;
	wfds = other.wfds;
	except = other.except;
	return *this;
}

Server::~Server()
{
	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	FD_ZERO(&except);
}

void		Server::makeServer(Socket&	soc)
{
	if (soc.s() != max_fd)
	{
		throw SocketExceptin();
	}
	if (bind(soc.s(), (const sockaddr *)&(soc.sin()), sizeof(soc.sin())) == -1)
	{
		throw BindError();
	}
	if (listen(soc.s(), 42) != 0)
	{
		throw ListenError();
	}
}

void		Server::runServer()
{
	while (1)
	{
		
	}
}

class	BindException : public std::exception
{
	public:
		virtual const char* what(void) const throw()
		{
			return "Bind Error\n";
		}
};

class	ListenException : public std::exception
{
	public:
		virtual const char* what(void) const throw()
		{
			return "Listen Error\n";
		}
};

