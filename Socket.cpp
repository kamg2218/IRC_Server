#include <iostream>
#include "include/Socket.hpp"

Socket::Socket(void)
	: _sd(-1), _port(0), _len(sizeof(_sin)), _proto(0)
{
}

Socket::Socket(Socket const& other)
	: _sd(-1), _port(0), _len(sizeof(_sin)), _proto(0)
{
	*this = other;
}

Socket&		Socket::operator=(Socket const& other)
{
	if (this == &other)
		return *this;
	return *this;
}

Socket::~Socket(void)
{
	if (_sd > 2)
		close(_sd);
}

void		Socket::setSd(int sd)
{
	_sd = sd;
}
int			Socket::sd(void) const
{
	return _sd;
}
unsigned int&	Socket::port(void)
{
	return _port;
}
unsigned int&	Socket::len(void)
{
	return _len;
}
sockaddr_in&	Socket::sin(void)
{
	return _sin;
}
protoent*		Socket::proto(void)
{
	return _proto;
}

int				Socket::makeSocket(unsigned int port)
{
	int			on;

	_proto = getprotobyname("tcp");
	if (!_proto)
		throw protoException();
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(port);
	_sin.sin_addr.s_addr = htonl(INADDR_ANY);
	_sd = socket(AF_INET, SOCK_STREAM, _proto->p_proto);
	if (_sd == -1)
		throw socketException();
	on = 1;
	if (setsockopt(_sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
		throw socketException();
	return _sd;
}

int				Socket::makeSocket(unsigned int port, unsigned long addr)
{
	int			on;

	_proto = getprotobyname("tcp");
	if (!_proto)
		throw protoException();
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(port);
	_sin.sin_addr.s_addr = htonl(addr);
	_sd = socket(AF_INET, SOCK_STREAM, _proto->p_proto);
	if (_sd == -1)
		throw socketException();
	on = 1;
	if (setsockopt(_sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
		throw socketException();
	return _sd;
}

void			Socket::makeNonBlocking(void)
{
	if (fcntl(_sd, F_SETFL, O_NONBLOCK) == -1)
		throw socketException();
}

const char*		Socket::protoException::what(void) const throw()
{
	return "Proto Error\n";
}

const char*		Socket::socketException::what(void) const throw()
{
	return "Socket Error\n";
}
