#ifndef FT_SESSION_HPP
# define FT_SESSION_HPP

#include <string>
#include <sys/select.h>
#include "Server.hpp"
#include <sys/resource.h>
#include <string>
#include <map>
#include <unistd.h>
#include "Message.hpp"

class Session
{
	private:
		User	*user;
		int		fd;
		//std::string buffer;
		Message buffer;
		std::string msg;

	public:
		typedef Session* 	pointer;
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
				buffer.insert(buf, r);
				for (std::map<int, Session*>::iterator it = ms.begin() ; it != ms.end() ; ++it)
				{
					if (buffer.gotFullMsg())
					{
						if ((*it).first != fd)
						{
							r = send((*it).first, buffer.getMessage().c_str(), buffer.msglen(), 0);	
							std::cout << r << "bytes : sent to " << (*it).first << "\n";
							buffer.reset();
						}
					}
				}
				return (false);
			}
		}
};

#endif
