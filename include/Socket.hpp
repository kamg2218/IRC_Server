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
		Socket(void);
		Socket(const Socket& other);
		Socket&		operator=(const Socket& other);
		~Socket(void);
		int					sd(void) const;
		unsigned int&		port(void);
		unsigned int&		len(void);
		sockaddr_in&		sin(void);
		protoent*			proto(void);
		void		setSd(int sd);
		void		makeNonBlocking(void);
		int			makeSocket(unsigned int port);
		int			makeSocket(unsigned int port, unsigned long addr);
		class		protoException : public std::exception
		{
			public:
				virtual const char* what(void) const throw();
		};
		class		socketException : public std::exception
		{
			public:
				virtual const char* what(void) const throw();
		};
};

#endif
