#include "include/MainServer.hpp"
#include "include/Session.hpp"
#include "include/Service.hpp"

MainServer::MainServer(void) :_name("ft_irc")
{
}

MainServer::MainServer(MainServer const& other)
{
	*this = other;
}


MainServer&	MainServer::operator=(const MainServer& other)
{
	if (this == &other)
		return *this;
	return *this;
}

MainServer::~MainServer(void)
{
}

void	MainServer::create(base const& bs)
{
	setPass(bs._password);
	_sd = _sock.makeSocket(bs._port);
	_sock.makeNonBlocking();
	if ((bind(_sd, (struct sockaddr *)&(_sock.sin()), sizeof(_sock.sin()))) == -1)
		throw BindException();
	if (listen(_sd, 42) < 0)
		throw ListenException();
}

void		MainServer::handleRead(std::map<int, Session*>::iterator temp)
{
	if (temp->second->handleRead())
		handleDecline(temp);
}

void	MainServer::handleAccept(void)
{
	int			on;
	int			cs;
	Session		*se;

	se = Session().create();
	cs = accept(_sd, (struct sockaddr*)&(se->soc().sin()), &(se->soc().len()));
	if (cs < 0)
		throw AcceptException();
	se->soc().setSd(cs);
	on = 1;
	if (setsockopt(cs, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
	{
		delete se;
		throw AcceptException();
	}
	std::cout << inet_ntoa(se->soc().sin().sin_addr) << ":" << ntohs(se->soc().sin().sin_port) << " is connected\n";
	_mSessions.insert(std::pair<int, Session*>(cs, se));
}

void		MainServer::handleDecline(std::map<int, Session*>::iterator const& pos)
{
	Session* temp;

	temp = (*pos).second;
	_mSessions.erase(pos);
	delete (temp);
}

int 	MainServer::socket(void) const
{
	return _sd;
}
std::string 	MainServer::name(void) const
{
	return _name;
}

std::map<int, Session*>&	MainServer::users(void)
{
	return _mSessions;
}

const std::map<int, Session*>&	MainServer::users(void) const
{
	return _mSessions;
}

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

std::string		MainServer::msgHeader()
{
	return (inet_ntoa(_sock.sin().sin_addr));
}

const char*		MainServer::BindException::what() const throw()
{
	return "Bind Error";
}

const char*		MainServer::ListenException::what() const throw()
{
	return "Listen Error\n";
}

const char*		MainServer::SocketException::what() const throw()
{
	return "Socket Error\n";
}

const char*		MainServer::AcceptException::what() const throw()
{
	return "Accpet Error";
}
