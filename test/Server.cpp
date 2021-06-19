#include "Server.hpp"

Server::Server()
{
}

Server::Server(int port)
{
	if (_soc.port() != port)
		_soc = Socket(port);
}

Server::Server(const Server& other)
{
	*this = other;
}

Server&		Server::operator=(const Server& other)
{
	if (this == &other)
		return *this;
	this->_soc = other._soc;
	return *this;
}

Server::~Server()
{
}

void		Server::makeServer()
{
	if (bind(_soc.s(), (const sockaddr *)&(_soc.sin()), sizeof(_soc.sin())) == -1)
	{
		throw BindError();
	}
	if (listen(_soc.s(), 42) != 0)
	{
		throw ListenError();
	}
}

Socket&		Server::soc()
{
	return _soc;
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

