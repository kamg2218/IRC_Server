#include "include/Service.hpp"
#include "include/User.hpp"

Service::Service()
{
	struct rlimit rlp;
	
	getrlimit(RLIMIT_NOFILE, &rlp);
	maxopen = rlp.rlim_cur;
}

void	Service::do_select(Server const& sv)
{
	res = 0;
	max = 0;
	FD_ZERO(&fd_read);
	FD_ZERO(&fd_write);
	FD_SET(sv.socket(), &fd_read);
	max = std::max(max, sv.socket());
	for (std::map<int, Session*>::const_iterator it = mSessions.begin() ; it != mSessions.end() ; ++it)
	{
		FD_SET((*it).second->socket(), &fd_read);
		max = std::max(max, (*it).second->socket());
		//write buffer?
	}
	res = select(max + 1, &fd_read, &fd_write, NULL, NULL);
}

void	Service::do_service(Server & sv)
{
	Session	*newclient;
	
	if (FD_ISSET(sv.socket(), &fd_read))
	{
		newclient = sv.handleAccept(this);
		std::map<int, Session*>::value_type res(newclient->socket(), newclient);
		mSessions.insert(res);
	}
	for (std::map<int, Session*>::iterator it = mSessions.begin(); it != mSessions.end() ; )
	{
		if (FD_ISSET((*it).first, &fd_read))
		{
			std::map<int, Session*>::iterator temp = it++;
			if ((*temp).second->handleRead(mSessions))
				deleteSession(temp);
		}
		else
			++it;
	}
}
void	Service::deleteSession(std::map<int, Session*>::iterator& pos)
{
	Session* temp;

	temp = (*pos).second;
	mSessions.erase(pos);
	delete (temp);
	std::cout << "client is removed\n";
}

int		Service::getMaxopen() const
{
	return (maxopen);
}
