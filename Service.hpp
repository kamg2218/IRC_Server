#ifndef FT_SERVICE_HPP
# define FT_SERVICE_HPP

#include "Session.hpp"
#include <map>
#include <sys/resource.h>
#include <sys/select.h>

class Service
{
	private:
		fd_set fd_read;
		fd_set fd_write;
		int max;
		int maxopen;
		int res;

	public:
	Service()
	{
		struct rlimit rlp;
		
		getrlimit(RLIMIT_NOFILE, &rlp);
		maxopen = rlp.rlim_cur;
	}
	void do_select(Server const& sv, std::map<int, Session*> const& ms)
	{
		res = 0;
		max = 0;
		FD_ZERO(&fd_read);
		FD_ZERO(&fd_write);
		FD_SET(sv.socket(), &fd_read);
		max = std::max(max, sv.socket());
		for (std::map<int, Session*>::const_iterator it = ms.begin() ; it != ms.end() ; ++it)
		{
			FD_SET((*it).second->socket(), &fd_read);
			max = std::max(max, (*it).second->socket());
			//write buffer?
		}
		res = select(max + 1, &fd_read, &fd_write, NULL, NULL);
	}



	void do_service(Server & sv, std::map<int, Session*> & ms)
	{
		Session *newclient;
		
		if (FD_ISSET(sv.socket(), &fd_read))
		{
			newclient = sv.handleAccept();
			std::map<int, Session*>::value_type res(newclient->socket(), newclient);
			ms.insert(res);
		}
		for (std::map<int, Session*>::iterator it = ms.begin(); it != ms.end() ; )
		{
			if (FD_ISSET((*it).first, &fd_read))
			{
				std::map<int, Session*>::iterator temp = it++;
				if ((*temp).second->handleRead(ms))
					ms.erase(temp);
			}
			else
				++it;
		}
	}
	int getRes() const
	{
		return (res);
	}
	void close(int fd);
	bool check_read(int fd)
	{
		return (FD_ISSET(fd, &fd_read));
	}
	bool check_write(int fd)
	{
		return (FD_ISSET(fd, &fd_write));
	}
	int getMaxopen() const
	{
		return (maxopen);
	}

};

#endif
