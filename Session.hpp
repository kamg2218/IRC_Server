#ifndef SESSION_HPP
# define SESSION_HPP

#include <string>
#include <sys/select.h>
#include <sys/resource.h>
#include <map>
#include <unistd.h>
#include "User.hpp"

class Session
{
	private:
		User	*user;
		int		fd;
		std::string msg;

	public:
		Session()
		{
			fd = -1;
			user = NULL;
		}
		Session(int csfd, struct sockaddr_in const& csinfo)
		{
			this->fd = csfd;
			user = new User("real", "nick", "host");
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
				buffer.insert(buf, r);
				if (!buffer.gotFullMsg())
					return (true);

				for (std::map<int, Session*>::iterator it = ms.begin() ; it != ms.end() ; ++it)
				{
					if ((*it).first != fd)
					{
						r = send((*it).first, buffer.getMessage().c_str(), buffer.msglen(), 0);	
					}
				}
				buffer.reset();
				reply("001");
				return (false);
			}
		}
};

#endif
