#include "include/Service.hpp"
#include "include/User.hpp"

Service::Service()
{
	struct rlimit rlp;
	
	getrlimit(RLIMIT_NOFILE, &rlp);
	maxopen = rlp.rlim_cur;
}

void	Service::do_set()
{
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	FD_ZERO(&fd_read);
	FD_SET(sv.socket(), &fd_read);
	max = sv.socket();
	for (std::map<int, Session*>::const_iterator it = mSessions.begin() ; it != mSessions.end() ; ++it)
	{
		//std::cout << "FD_SET " << it->first << std::endl;
		FD_SET(it->first, &fd_read);
		max = std::max(max, it->first);
	}
}

void	Service::do_select(MainServer const& sv)
{
	do_set();
	res = select(max + 1, &fd_read, NULL, NULL, &tv);
	if (res == -1)
	{
		throw SelectException();
	}
}

void	Service::do_service(MainServer & sv)
{
	Session	*newclient;

	if (res <= 0)
	{
		std::cout << "No data Exist\n";
		return ;
	}
	if (FD_ISSET(sv.socket(), &fd_read))
	{
		newclient = sv.handleAccept(this);
		std::cout << "client is accepted\n";
	}
	for (std::map<int, Session*>::iterator it = mSessions.begin(); it != mSessions.end() ; )
	{
		//std::cout << "client " << it->first << " is still alive.\n";
		std::map<int, Session*>::iterator temp = it++;
		if (FD_ISSET(temp->first, &fd_read))
		{
			//std::cout << "Read = " << temp->first << std::endl;
			if (temp->second->handleRead(mSessions, temp->first))
				sv.handleDecline(mSessions, temp);
		}
	}
}

int		Service::getMaxopen() const
{
	return (maxopen);
}
