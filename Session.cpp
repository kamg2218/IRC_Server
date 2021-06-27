#include "include/Session.hpp"
#include "include/User.hpp"
#include "include/Service.hpp"

Session::Session(int csfd, Service* svc)
	: user(this), service(svc)
{
	this->fd = csfd;
}

Session::Session(Service* svc)
	: user(this), service(svc)
{
	fd = -1;
}

Session::pointer	Session::create(Service* svc, int nbr)
{
	return (new Session(svc));
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
		request.insert(buf, r);
		if (!request.gotFullMsg())
			return (false);
		request.execute(&user);
		request.reset();
		//reply("001");
		return (false);
	}
}

void	Session::reply(std::string const& str)
{
	std::string res = str;	
	res += "\r\n";
	send(fd, res.c_str(), res.length(), 0);
}

Socket&	Session::soc() { return _soc; }
