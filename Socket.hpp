#ifndef FT_SOCKET_HPP
# define FT_SOCKET_HPP

#include "Fd.hpp"
#include <sys/resource.h>
#include <sys/select.h>

class Socket
{
	private:
		Fd *fds;
		fd_set fd_read;
		fd_set fd_write;
		int max;
		int maxopen;
		int res;

	public:
	Socket()
	{
		struct rlimit rlp;
		
		getrlimit(RLIMIT_NOFILE, &rlp);
		maxopen = rlp.rlim_cur;
		fds = new Fd[maxopen];
		for (int i = 0; i < maxopen ; i++)
			fds[i].clean();
	}
	void init()
	{
		res = 0;
		max = 0;
		FD_ZERO(&fd_read);
		FD_ZERO(&fd_write);
		for (int i = 0 ; i < maxopen ; i++)
		{
			if (fds[i].getType() != Fd::FREE)
			{
				FD_SET(i, &fd_read);
				max = std::max(max, i);
				if (fds[i].wbuf().length() > 0)
					FD_SET(i, &fd_write);
			}
		}
	}
	void do_select()
	{
		init();
		res = select(max + 1, &fd_read, &fd_write, NULL, NULL);
	}
	int getRes() const
	{
		return (res);
	}
	void	addServer(int fd, Server *p)
	{
		fds[fd].set(p);
	}
	void	addClient(int fd, User *p)
	{
		fds[fd].set(p);
	}
	void close(int fd);
};

#endif
