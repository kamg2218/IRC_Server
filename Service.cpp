#include "include/Service.hpp"
#include "include/User.hpp"

Service::Service()
{
	struct rlimit rlp;
	
	getrlimit(RLIMIT_NOFILE, &rlp);
	maxopen = rlp.rlim_cur;
}

void	Service::do_select(MainServer const& sv)
{
	res = 0;
	FD_ZERO(&fd_read);
	FD_SET(sv.socket(), &fd_read);
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	max = sv.socket();
	for (std::map<int, Session*>::const_iterator it = sv.users().begin() ; it != sv.users().end() ; ++it)
	{

		//std::cout << "FD_SET " << it->first << std::endl;

		FD_SET(it->first, &fd_read);
		max = std::max(max, it->first);
	}
	res = select(max + 1, &fd_read, NULL, NULL, &tv);
	if (res == -1)
	{
		throw SelectException();
	}
}

void	Service::do_service(MainServer & sv)
{
	//Session	*newclient;

	if (res <= 0)
	{

		//std::cout << "No data Exist\n";

		return ;
	}
	if (FD_ISSET(sv.socket(), &fd_read))
	{
		sv.handleAccept(this);
		std::cout << "client is accepted\n";
	}
	for (std::map<int, Session*>::iterator it = sv.users().begin(); it != sv.users().end() ; )
	{
		//std::cout << "client " << it->first << " is still alive.\n";
		std::map<int, Session*>::iterator temp = it++;
		if (FD_ISSET(temp->first, &fd_read))
		{
			//std::cout << "Read = " << temp->first << std::endl;
			sv.handleRead(temp);
		}
	}
}

int		Service::getMaxopen() const
{
	return (maxopen);
}
