#ifndef SOCKET_HPP
# define SOCKET_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <exception>

class	Socket
{
	private:
		int				_sd;
		unsigned int	_port;
		unsigned int	_len;
		sockaddr_in		_sin;
		protoent*		_proto;
	public:
		Socket();
		Socket(Socket& other);
		Socket&		operator=(Socket& other);
		~Socket();
		int				sd() const;
		unsigned int&	port();
		unsigned int&	len();
		sockaddr_in&	sin();
		protoent*		proto();
		void			setSd(int sd);
		void			makeNonBlocking();
		int				makeSocket(unsigned int port);
		int				makeSocket(unsigned int port, unsigned long addr);
		class			ProtoException : public std::exception
		{
			public:
				virtual const char* what(void) const throw()
				{
					return "Proto Error\n";
				}
		};
		class			SocketException : public std::exception
		{
			public:
				virtual const char* what(void) const throw()
				{
					return "Socket Error\n";
				}
		};
};

#endif
