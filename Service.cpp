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

Service::Service(const Service& other)
{
	*this = other;
}

Service&	Service::operator=(const Service& other)
{
	if (this == &other)
		return *this;
	this->max = other.max;
	this->ret = other.ret;
	this->rfds = other.rfds;
	this->wfds = other.wfds;
	this->except = other.except;
	this->tv = other.tv;
	return *this;
}

Service::~Service()
{
	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	FD_ZERO(&except);
}

void		Service::doSelect(Socket& soc, std::map<int, Socket>& mSockets)
{
	ret = select(max + 1, &rfds, &wfds, NULL, &tv);
	if (ret == -1)
	{
		std::perror("select Error = ");
		throw SelectException();
	}
	else if (ret)
	{
		check_rfds(soc, mSockets);
		check_wfds(soc, mSockets);
		//check_except(soc, mSockets);
	}
	else
	{
		std::cout << "No data within five seconds.\n";
	}
}

void		Service::handleAccept(Socket& soc, std::map<int, Socket>& mSockets)
{
	std::pair<int, Socket>	p;

	p.first = accept(soc.s(), (struct sockaddr*)&(p.second.sin()), &(p.second.len()));
	if (p.first == -1)
	{
		throw AcceptException();
	}
	mSockets.insert(p);
	FD_SET(p.first, &rfds);
	FD_SET(p.first, &wfds);
	//FD_SET(p.first, &except);
	if (max < p.first)
		max = p.first;
}

void		Service::set(Socket& soc, std::map<int, Socket>& mSockets)
{
	std::map<int, Socket>::const_iterator	it;

	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	FD_ZERO(&except);
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	max = soc.s();
	FD_SET(soc.s(), &rfds);
	FD_SET(soc.s(), &wfds);
	//FD_SET(soc.s(), &except);
	for (it = mSockets.begin(); it != mSockets.end(); it++)
	{
		std::cout << "it = " << it->first << std::endl;
		FD_SET((*it).first, &rfds);
		FD_SET((*it).first, &wfds);
		//FD_SET((*it).first, &except);
		if ((*it).first > max)
			max = (*it).first;
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
			std::cout << "i = " << i << ", max = " << max << std::endl;
			if (i == soc.s())
			{
				handleAccept(soc, mSockets);
			}
			else
			{
				memset(buf, 0, 1024);
				if (recv(i, buf, 1024, 0) <= 0)
				{
					FD_CLR(i, &rfds);
					mSockets.erase(i);
					continue ;
				}
				/*
				if (read(i, buf, 1024) <= 0)
				{
					FD_CLR(i, &rfds);
					mSockets.erase(i);
					continue ;
				}*/
				std::cout << "Message = " << buf << std::endl;
				//write(i, "end", 1024);
				//send(i, "end", 1024, 0);
				FD_SET(i, &wfds);
				if (strncmp(buf, "quit", 4) == 0)
				{
					FD_CLR(i, &rfds);
					mSockets.erase(i);
				}
			}
		}
	}
}

void		Service::check_wfds(Socket& soc, std::map<int, Socket>& mSockets)
{
	int		i;
	char	buf[1024];

	for (i = 0; i < max + 1; i++)
	{
		if (FD_ISSET(i, &wfds))
		{
			if (i == soc.s())
			{
				std::cout << "wfds = " << i << std::endl;
			}
			else
			{
				std::cout << "wfds = " << i << std::endl;
				//memset(buf, 0, 1024);
				//recv(i, buf, 1024, 0);
				send(i, "end", 1024, 0);
			}
		}
	}
}

