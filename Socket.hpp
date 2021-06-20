#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <exception>

class Socket
{
	private:
		int				_s;
		unsigned int	_len;
		int				_port;
		sockaddr_in		_sin;
		protoent*		_proto;
	public:
		Socket();
		Socket(int port);
		Socket(int port, unsigned long addr);
		Socket(const Socket& other);
		Socket&		operator=(const Socket& other);
		~Socket();
		//void			makeSocket(int port);
		int&			s();
		int&			port();
		unsigned int&	len();
		sockaddr_in&	sin();
		protoent*		proto();
		//class ProtoException;
		//class SocketException;
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

};

#endif
