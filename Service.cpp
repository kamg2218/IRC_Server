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

void		Service::doSelect(Socket& soc, std::map<int, User>& mSockets)
{
	ret = select(max + 1, &rfds, &wfds, NULL, &tv);
	if (ret == -1)
	{
		//std::perror("select Error = ");
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

void		Service::handleAccept(Socket& soc, std::map<int, User>& mSockets)
{
	std::pair<int, User>	p;

	p.first = accept(soc.s(), (struct sockaddr*)&(p.second.soc().sin()), &(p.second.soc().len()));
	if (p.first == -1)
	{
		throw AcceptException();
	}
	mSockets.insert(p);
	FD_SET(p.first, &rfds);
	if (max < p.first)
		max = p.first;
}

void		Service::set(Socket& soc, std::map<int, User>& mSockets)
{
	std::map<int, User>::const_iterator		it;

	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	FD_ZERO(&except);
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	max = soc.s();
	FD_SET(soc.s(), &rfds);
	for (it = mSockets.begin(); it != mSockets.end(); it++)
	{
		std::cout << "it = " << it->first << std::endl;
		FD_SET(it->first, &rfds);
		if (it->first > max)
			max = it->first;
	}
}

void		Service::check_rfds(Socket& soc, std::map<int, User>& mSockets)
{
	char	buf[1024];
	int		i;

	std::cout << "Check_rfds\n";
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
				if (recv(i, buf, 1024, 0) <= 0)
				{
					close(mSockets.find(i)->second.soc().s());
					mSockets.erase(i);
					continue ;
				}
				std::cout << "Message = " << buf << std::endl;
				//send(i, "end", 1024, 0);
				FD_SET(i, &wfds);
				if (strncmp(buf, "quit", 4) == 0)
				{
					close(mSockets.find(i)->second.soc().s());
					mSockets.erase(i);
				}
			}
		}
	}
}

void		Service::check_wfds(Socket& soc, std::map<int, User>& mSockets)
{
	int		i;

	std::cout << "Check_wfds\n";
	for (i = 0; i < max + 1; i++)
	{
		if (FD_ISSET(i, &wfds))
		{
			/*
			if (i == soc.s())
			{
				std::cout << "wfds soc = " << soc.s() << std::endl;
			}
			else
			{
			*/
			std::cout << "wfds = " << i << std::endl;
			send(i, "end", 1024, 0);
			//}
		}
	}
}

