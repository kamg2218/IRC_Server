#include "Socket.hpp"

Socket::Socket()
	: _s(0)
{
	_proto = getprotobyname("tcp");
	if (!_proto)
	{
		throw ProtoException();
	}
	_s = socket(AF_INET, SOCK_STREAM, _proto->p_proto);
	if (_s == -1)
	{
		throw SocketException();
	}
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(80);
	_sin.sin_addr.s_addr = htonl(INADDR_ANY);
}

Socket::Socket(int port)
	: _s(0)
{
	_proto = getprotobyname("tcp");
	if (!_proto)
	{
		throw ProtoException();
	}
	_s = socket(AF_INET, SOCK_STREAM, _proto->p_proto);
	if (_s == -1)
	{
		throw SocketException();
	}
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(port);
	_sin.sin_addr.s_addr = htonl(INADDR_ANY);
}

Socket::Socket(int port, unsigned long addr)
	: _s(0)
{
	_proto = getprotobyname("tcp");
	if (!_proto)
	{
		throw ProtoException();
	}
	_s = socket(AF_INET, SOCK_STREAM, _proto->p_proto);
	if (_s == -1)
	{
		throw SocketException();
	}
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(port);
	_sin.sin_addr.s_addr = htonl(addr);
}

Socket::Socket(const Socket& other)
{
	*this = other;
}

Socket&		Socket::operator=(const Socket& other)
{
	if (this == &other)
	{
		return *this;
	}
	close(this->_s);
	this->_s = other._s;
	this->_sin = other._sin;
	this->_proto = other._proto;
	return *this;
}

int&			Socket::s()
{
	return _s;
}

unsigned int&	Socket::len()
{
	return _len;
}

int&			Socket::port()
{
	return _port;
}

sockaddr_in&	Socket::sin()
{
	return _sin;
}

protoent*		Socket::proto()
{
	return _proto;
}

Socket::~Socket()
{
	close(_s);
}
/*
class	ProtoException : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return "Proto Error\n";
		}
};

class	SocketException : public std::exception
{
	public:
		virtual const char *what(void) const throw()
		{
			return "Socket Error\n";
		}
};
*/
