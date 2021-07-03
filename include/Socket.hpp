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
		int				_port;
		unsigned int	_len;
		sockaddr_in		_sin;
		protoent*		_proto;
	public:
		Socket();
		Socket(Socket& other);
		Socket&		operator=(Socket& other);
		~Socket();
		int				sd() const;
		int				port();
		unsigned int	len();
		sockaddr_in&	sin();
		protoent*		proto();
		void			setSd(int sd);
		void			setPort(int port);
		void			setLen(unsigned int len);
		void			setSockaddr(sockaddr_in sin);
		void			setProto(protoent* proto);
		void			makeNonBlocking();
		void			makeSocket(int port);
		void			makeSocket(int port, unsigned long addr);
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
