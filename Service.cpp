#include "include/Service.hpp"
#include "include/User.hpp"
#include "include/Frame.hpp"

Service::Service(void)
{
}

Service::Service(const Service& other)
{
	*this = other;
}

Service&	Service::operator=(const Service& other)
{
	if (this == &other)
		return *this;
	this->_fdRead = other._fdRead;
	this->_fdWrite = other._fdWrite;
	this->_max = other._max;
	this->_res = other._res;
	this->_tv = other._tv;
	return *this;
}

Service::~Service(void)
{
}

void	Service::doSelect(MainServer const& sv)
{
	_res = 0;
	FD_ZERO(&_fdRead);
	FD_SET(sv.socket(), &_fdRead);
	_tv.tv_sec = 5;
	_tv.tv_usec = 0;
	_max = sv.socket();
	for (std::map<int, Session*>::const_iterator it = sv.users().begin() ; it != sv.users().end() ; ++it)
	{
		FD_SET(it->first, &_fdRead);
		_max = std::max(_max, it->first);
	}
	_res = select(_max + 1, &_fdRead, NULL, NULL, &_tv);
	if (_res == -1)
		throw selectException();
}

void	Service::doService(MainServer & sv)
{
	if (_res <= 0)
		return ;
	if (FD_ISSET(sv.socket(), &_fdRead))
		sv.handleAccept(this);
	for (std::map<int, Session*>::iterator it = sv.users().begin(); it != sv.users().end() ; )
	{
		std::map<int, Session*>::iterator temp = it++;
		if (FD_ISSET(temp->first, &_fdRead))
			sv.handleRead(temp);
		else
			sendPing(temp->second);
	}
}

void	Service::sendPing(Session *ss)
{
	std::string	msg;
	std::vector<std::string>	v;

	if (ss->ping() == false)
	{
		v.insert(v.end(),"QUIT");
		v.insert(v.end(), ":" + std::to_string(ss->soc().sd()) + " client is missing");
		Frame::instance()->cmdQuit(ss, v);
		return ;
	}
	msg = "PING ";
	msg += ss->user().nick();
	msg += "\r\n";
	send(ss->soc().sd(), msg.c_str(), msg.length(), 0);
	ss->setPing(false);
}

const char*	Service::selectException::what(void) const throw()
{
	return "Select Error\n";
}
