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
	FD_SET(sv.socket(), &fd_read);
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	max = std::max(max, sv.socket());
	for (std::map<int, Session*>::const_iterator it = mSessions.begin() ; it != mSessions.end() ; ++it)
	{
		FD_SET(it->first, &fd_read);
		max = std::max(max, it->first);
		//write buffer?
	}
	res = select(max + 1, &fd_read, NULL, NULL, &tv);
	if (res == -1)
	{
		throw SelectException();
	}
}

void	Service::do_service(Server & sv)
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
		std::map<int, Session*>::value_type res(newclient->soc().fd(), newclient);
		mSessions.insert(res);
		newclient->soc().makeNonBlocking();
		std::cout << "client " << res.first << " is accepted\n";
	}
	std::cout << "userMap = " << mSessions.size() << std::endl;
	for (std::map<int, Session*>::iterator it = mSessions.begin(); it != mSessions.end() ; )
	{
		std::cout << "client " << it->first << " is still alive.\n";
		std::map<int, Session*>::iterator temp = it++;
		if (FD_ISSET(it->first, &fd_read))
		{
			std::cout << "Read = " << it->first << std::endl;
			if ((*temp).second->handleRead(mSessions))
				deleteSession(temp);
		}
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
