#include <iostream>
#include "include/Socket.hpp"

Socket::Socket()
	: _sd(0), _port(0), _len(0), _proto(0)
{
}

Socket::Socket(Socket& other)
	: _sd(0), _port(0), _len(0), _proto(0)
{
	*this = other;
}

Socket&		Socket::operator=(Socket& other)
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

Socket::~Socket()
{
}

void		Socket::setSd(int sd) { _sd = sd; }
void		Socket::setPort(int port) { _port = port; }
void		Socket::setLen(unsigned int len) { _len = len; }
void		Socket::setSockaddr(sockaddr_in sin) { _sin = sin; }
void		Socket::setProto(protoent* proto) { _proto = proto; }

int			Socket::sd() const
{
	return _sd;
}

int			Socket::port()
{
	return _port;
}

unsigned int	Socket::len()
{
	return _len;
}

sockaddr_in&	Socket::sin()
{
	return _sin;
}

protoent*		Socket::proto()
{
	return _proto;
}

void			Socket::makeSocket(int port)
{
	int			on;

	_proto = getprotobyname("tcp");
	if (!_proto)
	{
		throw ProtoException();
	}
	_sd = socket(AF_INET, SOCK_STREAM, _proto->p_proto);
	if (_sd == -1)
	{
		throw SocketException();
	}
	on = 1;
	if (setsockopt(_sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
	{
		throw SocketException();
	}
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(port);
	_sin.sin_addr.s_addr = htonl(INADDR_ANY);
}

void			Socket::makeSocket(int port, unsigned long addr)
{
	int			on;

	_proto = getprotobyname("tcp");
	if (!_proto)
	{
		throw ProtoException();
	}
	_sd = socket(AF_INET, SOCK_STREAM, _proto->p_proto);
	if (_sd == -1)
	{
		throw SocketException();
	}
	on = 1;
	if (setsockopt(_sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
	{
		throw SocketException();
	}
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(port);
	_sin.sin_addr.s_addr = htonl(addr);
}

void			Socket::makeNonBlocking()
{
	int			flag;

	std::cout << "sd = " << _sd << std::endl;
	flag = fcntl(_sd, F_GETFL, 0);
	if (fcntl(_sd, F_SETFL, flag | O_NONBLOCK) == -1)
	{
		throw SocketException();
	}
}
