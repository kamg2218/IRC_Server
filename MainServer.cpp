#include "include/MainServer.hpp"
#include "include/Session.hpp"
#include "include/Service.hpp"

const char*		MainServer::BindException::what() const throw()
{
	return ("Bind Error");
}

const char*		MainServer::ListenException::what() const throw()
{
	return ("Listen Error\n");
}

const char*		MainServer::SocketException::what() const throw()
{
	return ("Socket Error\n");
}

const char*		MainServer::AcceptException::what() const throw()
{
	return ("Accpet Error");
}

MainServer::MainServer() {}

void	MainServer::create(unsigned int port)
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

Session*	MainServer::handleAccept(Service* p)
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
void		MainServer::handleDecline(User& usr)
{
}

void		MainServer::handleDecline(Channel& chn)
{
}

void		MainServer::handleDecline(MainServer& svr)
{
}
*/
int 	MainServer::socket() const
{
	return (sockfd);
}
