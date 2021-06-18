#ifndef FT_SESSION_HPP
# define FT_SESSION_HPP

#include <string>
#include <sys/select.h>
#include "Server.hpp"
#include <sys/resource.h>
#include "Shared_Ptr.hpp"
#include <string>
#include <map>
#include <unistd.h>

class Session
{
	private:
		User	*user;
		int		fd;
		std::string buffer;
		std::string msg;

	public:
		Session(int csfd, struct sockaddr_in const& csinfo)
		{
			this->fd = csfd;
			user = new User("real", "nick", "host");
		}
		Session()
		{
			fd = -1;
			user = NULL;
		}
		typedef Session* 	pointer;
		static pointer create()
		{
			return (new Session());
		}
		int socket() const
		{
			return (fd);
		}
		bool handleRead(std::map<int, Session*> & ms)
		{
			int r;
			char buf[101];
			r = recv(fd, buf, 100, 0);
			if (r <= 0)
			{
				std::cout << "client gone\n";
				return (true);
			}
			else
			{
				for (std::map<int, Session*>::iterator it = ms.begin() ; it != ms.end() ; ++it)
				{
					if ((*it).first != fd)
						r = send((*it).first, buf, strlen(buf), 0);	
				}
				return (false);
			}
		}
};

#endif
