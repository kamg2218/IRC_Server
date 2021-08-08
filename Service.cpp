#include "include/Service.hpp"
#include "include/User.hpp"
#include "include/Frame.hpp"

Service::Service(void)
{
}

Service::Service(Service const& other)
{
	*this = other;
}

Service&		Service::operator=(Service const& other)
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

void		Service::doSelect(MainServer const& sv)
{
	std::map<int, Session*>::const_iterator		it;

	_res = 0;
	FD_ZERO(&_fdRead);
	FD_SET(sv.socket(), &_fdRead);
	_tv.tv_sec = 5;
	_tv.tv_usec = 0;
	_max = sv.socket();
	for (it = sv.users().begin() ; it != sv.users().end() ; ++it)
	{
		FD_SET(it->first, &_fdRead);
		_max = std::max(_max, it->first);
	}
	_res = select(_max + 1, &_fdRead, NULL, NULL, &_tv);
	if (_res == -1)
		throw selectException();
}

void		Service::doService(MainServer & sv)
{
	std::map<int, Session*>::iterator	it;
	std::map<int, Session*>::iterator	temp;

	if (_res < 0)
		return ;
	else if (_res == 0)
	{
		for (it = sv.users().begin(); it != sv.users().end() ;)
		{
			temp = it++;
			sendPing(temp->second);
		}
	}
	else
	{
		for (it = sv.users().begin(); it != sv.users().end() ; )
		{
			temp = it++;
			if (FD_ISSET(temp->first, &_fdRead))
			{
				temp->second->setTime(0);
				sv.handleRead(temp);
			}
			else
				sendPing(temp->second);
		}
		if (FD_ISSET(sv.socket(), &_fdRead))
			sv.handleAccept();
	}
}

/*
   * When there is no data, send Ping Message
 */
void		Service::sendPing(Session *ss)
{
	std::string	msg;
	std::vector<std::string>	v;

	if (ss->time() != 0 && std::difftime(std::time(0), ss->time()) > 5)
	{
		v.insert(v.end(),"QUIT");
		v.insert(v.end(), ":" + std::to_string(ss->soc().sd()) + " client is missing");
		Frame::instance()->cmdQuit(ss, v);
		return ;
	}
	else if (ss->time() != 0)
		return ;
	msg = "PING ";
	msg += ss->user().nick();
	msg += "\r\n";
	send(ss->soc().sd(), msg.c_str(), msg.length(), 0);
	std::cout << "send ping to " << ss->user().nick() << std::endl;
	ss->setTime(std::time(0));
}

const char*		Service::selectException::what(void) const throw()
{
	return "Select Error\n";
}
