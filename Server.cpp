#include "include/Server.hpp"
#include "include/Session.hpp"
#include "include/Service.hpp"

const char*		Server::BindException::what() const throw()
{
	return ("Bind Error");
}

const char*		Server::ListenException::what() const throw()
{
	return ("Listen Error\n");
}

const char*		Server::SocketException::what() const throw()
{
	return ("Socket Error\n");
}

const char*		Server::AcceptException::what() const throw()
{
	return ("Accpet Error");
}

Server::Server() {}

void	Server::create(unsigned int port)
{
	int		on;
	struct	protoent	*pe;
	struct	sockaddr_in	sin;

	pe = getprotobyname("tcp");
	sockfd = ::socket(PF_INET, SOCK_STREAM, pe->p_proto);
	if (!sockfd)
	{
		throw SocketException();
	}
	on = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
	{
		throw SocketException();
	}
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(port);
	if ((bind(sockfd, (struct sockaddr *)&sin, sizeof(sin))) == -1)
	{
		throw BindException();
	}
	if (listen(sockfd, 42) < 0)
	{
		throw ListenException();
	}
}

Session*	Server::handleAccept(Service* p)
{
	int		on;
	int		cs;
	struct	sockaddr_in	csin;
	socklen_t	csin_len;

	csin_len = sizeof(csin);
	std::cout << "sockfd = " << sockfd << std::endl;
	cs = accept(sockfd, (struct sockaddr*)&csin, &csin_len);
	std::cout << "cs = " << cs << std::endl;
	if (cs < 0)
	{
		throw AcceptException();
	}
	on = 1;
	if (setsockopt(cs, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
	{
		throw AcceptException();
	}
	std::cout << inet_ntoa(csin.sin_addr) << ":" << ntohs(csin.sin_port) << " is connected\n";

	//return (new Session(cs, p));
	Session		*se;
	se = new Session(cs, p);
	se->soc().makeNonBlocking();
	p->users().insert(std::pair<int, Session*>(cs, se));
	return NULL;
}
/*
void		Server::handleDecline(User& usr)
{
}

void		Server::handleDecline(Channel& chn)
{
}

void		Server::handleDecline(Server& svr)
{
}
*/
int 	Server::socket() const
{
	return (sockfd);
}
