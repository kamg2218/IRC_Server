#include "include/Socket.hpp"

Socket::Socket()
	: _fd(0), _port(0), _len(0), _proto(0)
{
}

Socket::Socket(Socket& other)
	: _fd(0), _port(0), _len(0), _proto(0)
{
	*this = other;
}

Socket&		Socket::operator=(Socket& other)
{
	if (this == &other)
		return *this;
	this->_fd = other._fd;
	this->_port = other._port;
	this->_len = other._len;
	this->_sin = other._sin;
	this->_proto = other._proto;
	return *this;
}

Socket::~Socket()
{
}

int			Socket::fd() const
{
	return _fd;
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
	_fd = socket(AF_INET, SOCK_STREAM, _proto->p_proto);
	if (_fd == -1)
	{
		throw SocketException();
	}
	on = 1;
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
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
	_fd = socket(AF_INET, SOCK_STREAM, _proto->p_proto);
	if (_fd == -1)
	{
		throw SocketException();
	}
	on = 1;
	if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
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

	flag = fcntl(_fd, F_GETFL, 0);
	if (fcntl(_fd, F_SETFL, flag | O_NONBLOCK) == -1)
	{
		throw SocketException();
	}
}
/*
class			Socket::ProtoException : public std::exception
{
	public:
		virtual const char* what(void) const throw()
		{
			return "Proto Error\n";
		}
};

class			Socket::SocketException : public std::exception
{
	public:
		virtual const char* what(void) const throw()
		{
			return "Socket Error\n";
		}
};
*/
