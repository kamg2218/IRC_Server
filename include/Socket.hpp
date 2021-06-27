#ifndef SOCKET_HPP
# define SOCKET_HPP

class	Socket
{
	private:
		int				_fd;
		int				_port;
		unsigned int	_len;
		sockaddr_in		_sin;
		protoent*		_proto;
	public:
		Socket();
		Socket(Socket& other);
		Socket&		operator=(Socket& other);
		~Socket();
		int				fd();
		int				port();
		unsigned int	len();
		sockaddr_in&	sin();
		protoent*		proto();
		void			makeNonBlocking();
		void			makeSocket(int port);
		void			makeSocket(int port, unsigned long addr);
		class			ProtoException;
		class			SocketException;
};

#endif
