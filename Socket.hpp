#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

class Socket
{
	private:
		int				_s;
		sockaddr_in		_sin;
		protoent*		_proto;
	public:
		Socket();
		Socket(int port);
		Socket(int port, unsigned long addr);
		Socket(const Socket& other);
		Socket&		operator=(const Socket& other);
		~Socket() { close(_s); }
		int&			s();
		sockaddr_in&	sin();
		protoent*		proto();
};

Socket::Socket() : _s(0) {
	_proto = getprotobyname("tcp");
	if (!_proto){
		std::cout << "Error" << std::endl;
		exit(0);
	}
	_s = socket(AF_INET, SOCK_STREAM, _proto->p_proto);
	if (_s == -1){
		std::cout << "Socket Error" << std::endl;
		exit(0);
	}
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(80);
	_sin.sin_addr.s_addr = htonl(INADDR_ANY);
}

Socket::Socket(int port) : _s(0) {
	_proto = getprotobyname("tcp");
	if (!_proto){
		std::cout << "Error" << std::endl;
		exit(0);
	}
	_s = socket(AF_INET, SOCK_STREAM, _proto->p_proto);
	if (_s == -1){
		std::cout << "Socket Error" << std::endl;
		exit(0);
	}
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(port);
	_sin.sin_addr.s_addr = htonl(INADDR_ANY);
}

int&			Socket::s() { return _s; }
sockaddr_in&	Socket::sin() { return _sin; }
protoent*		Socket::proto() { return _proto; }

//Socket::~Socket() { close(_s); }

#endif
