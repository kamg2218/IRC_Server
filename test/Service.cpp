#include "Service.hpp"

Service::Service()
	: max(0), ret(0)
{
	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	FD_ZERO(&except);
	tv.tv_sec = 5;
	tv.tv_usec = 0;
}

Service::Service(const Socket& soc)
	: max(soc.s()), ret(0)
{
	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	FD_ZERO(&except);
	FD_SET(soc.s(), &rfds);
	FD_SET(soc.s(), &wfds);
	FD_SET(soc.s(), &except);
	tv.tv_sec = 5;
	tv.tv_usec = 0;
}

Service::Service(const Service& other)
{
	*this = other;
}

Service&	operator=(const Service& other)
{
	if (this == &other)
		return *this;
	this->max = other.max;
	this->ret = other.ret;
	this->rfds = other.rfds;
	this->wfds = other.wfds;
	this->except = other.except;
	this->tv = other.tv;
}

Service::~Service()
{
	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	FD_ZERO(&except);
}

void		Service::doSelect(Socket& soc, std::map<int, Socket>& mSockets)
{
	ret = select(max + 1, &rfds, &wfds, &except, &tv);
	if (ret == -1)
	{
		throw SelectException();
	}
	else if (ret)
	{
		check_rfds(soc, mSockets);
		//check_wfds(soc, mSockets);
		//check_except(soc, mSockets);
	}
	else
	{
		std::cout << "No data within five seconds.\n";
	}
	set(soc, mSockets);
}

void		Service::handleAccept(Socket& soc, std::map<int, Socket>& mSockets)
{
	std::pair<int, Socket>	p;

	p.first = accept(soc.s(), (struct sockaddr*)&(p.second.sin()), &p.second.len());
	if (p.first == -1)
	{
		throw AcceptExceptin();
	}
	mSockets.insert(p);
	FD_SET(p.first, &rfds);
	if (max < p.first)
		max = p.first();
}

void		Service::set(Socket& soc, std::map<int, Socket>& mSockets)
{
	std::map<int, Socket>::const_iterator	it;

	max = 0;
	FD_SET(soc.s(), &rfds);
	FD_SET(soc.s(), &wfds);
	FD_SET(soc.s(), &except);
	for (it = mSockets.begin(); it != end(); it++)
	{
		FD_SET((*it).second.s(), &rfds);
		FD_SET((*it).second.s(), &wfds);
		FD_SET((*it).second.s(), &except);
		if ((*it).second.s() > max)
			max = (*it).second.s();
	}
}

void		Service::check_rfds(Socket& soc, std::map<int, Socket>& mSockets)
{
	int		i;
	char	buf[1024];

	for (i = 0; i < max + 1; i++)
	{
		if (FD_ISSET(i, &rfds))
		{
			if (i == soc.s())
			{
				handleAccept(soc, mSockets);
			}
			else
			{
				memset(buf, 0, 1024);
				if (read(i, buf, 1024) <= 0)
				{
					mSockets.erase(i);
				}
				std::cout << "Message = " << buf << std::endl;
				write(i, "end", 1024);
				if (strncmp(buf, "quit", 4) == 0)
				{
					mSockets.erase(i);
				}
			}
		}
	}
}

class		AcceptException : public std::exception
{
	public:
		virtual const char* what(void) const throw()
		{
			return "Accept Error\n";
		}
};

class		SelectException : public std::exception
{
	public:
		virtual const char* what(void) const throw()
		{
			return "Select Error\n";
		}
};

