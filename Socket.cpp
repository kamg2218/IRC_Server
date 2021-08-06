#include <iostream>
#include "include/Socket.hpp"

Socket::Socket(void)
	: _sd(0), _port(0), _len(0), _proto(0)
{
}

Socket::Socket(const Socket& other)
	: _sd(0), _port(0), _len(0), _proto(0)
{
	*this = other;
}

Socket&		Socket::operator=(const Socket& other)
{
	if (this == &other)
		return *this;
	this->_sd = other._sd;
	this->_port = other._port;
	this->_len = other._len;
	this->_sin = other._sin;
	this->_proto = other._proto;
	return *this;
}

Socket::~Socket(void)
{
}

void		Socket::setSd(int sd) { _sd = sd; }
int			Socket::sd(void) const { return _sd; }
unsigned int&	Socket::port(void) { return _port; }
unsigned int&	Socket::len(void) { return _len; }
sockaddr_in&	Socket::sin(void) { return _sin; }
protoent*		Socket::proto(void){ return _proto;}

int				Socket::makeSocket(unsigned int port)
{
	int			on;

	_proto = getprotobyname("tcp");
	if (!_proto)
		throw protoException();
	_sd = socket(AF_INET, SOCK_STREAM, _proto->p_proto);
	if (_sd == -1)
		throw socketException();
	on = 1;
	if (setsockopt(_sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
		throw socketException();
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(port);
	_sin.sin_addr.s_addr = htonl(INADDR_ANY);
	return _sd;
}

int				Socket::makeSocket(unsigned int port, unsigned long addr)
{
	int			on;

	_proto = getprotobyname("tcp");
	if (!_proto)
		throw protoException();
	_sd = socket(AF_INET, SOCK_STREAM, _proto->p_proto);
	if (_sd == -1)
		throw socketException();
	on = 1;
	if (setsockopt(_sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
		throw socketException();
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(port);
	_sin.sin_addr.s_addr = htonl(addr);
	return _sd;
}

void			Socket::makeNonBlocking(void)
{
	int			flag;

	flag = fcntl(_sd, F_GETFL, 0);
	if (fcntl(_sd, F_SETFL, flag | O_NONBLOCK) == -1)
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
