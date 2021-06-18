#include "include/Session.hpp"

Session::Session(int csfd, struct sockaddr_in const& csinfo)
{
	this->fd = csfd;
	user = new User("real", "nick", "host");
}

Session::Session()
{
	fd = -1;
	user = NULL;
}

Session::pointer	Session::create()
{
	return (new Session());
}

int		Session::socket() const
{
	return (fd);
}

bool	Session::handleRead(std::map<int, Session*> & ms)
{
	int		r;
	char	buf[101];

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

void	Session::reply(std::string const& str)
{
	std::string res = str;	
	res += "\r\n";
	send(fd, res.c_str(), res.length(), 0);
}
