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
	_sd = _sock.makeSocket(port);
	if ((bind(_sd, (struct sockaddr *)&(_sock.sin()), sizeof(_sock.sin()))) == -1)
	{
		throw BindException();
	}
	if (listen(_sd, 42) < 0)
	{
		throw ListenException();
	}
}

void		MainServer::handleRead(std::map<int, Session*>::iterator temp)
{
	if (temp->second->handleRead(mSessions, temp->first))
		handleDecline(temp);
}


void	MainServer::handleAccept(Service* p)
{
	int			on;
	int			cs;

	Session		*se;

	se = Session().create();
	cs = accept(_sd, (struct sockaddr*)&(se->soc().sin()), &(se->soc().len()));
	if (cs < 0)
	{
		throw AcceptException();
	}
	se->soc().setSd(cs);

	se->soc().makeNonBlocking();

	on = 1;
	if (setsockopt(cs, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
	{
		delete se;
		throw AcceptException();
	}
	std::cout << inet_ntoa(se->soc().sin().sin_addr) << ":" << ntohs(se->soc().sin().sin_port) << " is connected\n";

	mSessions.insert(std::pair<int, Session*>(cs, se));
}

void		MainServer::handleDecline(std::map<int, Session*>::iterator& pos)
{
	Session* temp;

	temp = (*pos).second;
	mSessions.erase(pos);
	delete (temp);
	std::cout << "client is removed\n";
}

int 	MainServer::socket() const { return _sd; }

std::map<int, Session*>&	MainServer::users() { return mSessions; }

const std::map<int, Session*>&	MainServer::users() const { return mSessions; }

bool	MainServer::checkPass(std::string const& pass)
{
	if (_pass == pass)
		return true;
	return false;
}

void	MainServer::setPass(std::string const& pass)
{
	_pass = pass;
}
