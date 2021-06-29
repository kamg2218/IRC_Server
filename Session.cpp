#include "include/Session.hpp"
#include "include/User.hpp"
#include "include/Service.hpp"

Session::Session(int csfd, Service* svc)
	: user(this)
{
	//std::cout << "csfd = " << csfd << std::endl;
	_soc.setFd(csfd);
}

Session::Session(Service* svc)
	: user(this)
{
	//_soc.makeSocket(80);
	//fd = -1;
}

Session::pointer	Session::create(Service* svc, int nbr)
{
	return (new Session(svc));
}

int		Session::socket() const
{
	return (_soc.fd());
}

bool	Session::handleRead(std::map<int, Session*> & ms, int fd)
{
	int		r;
	char	buf[1024];

	//std::cout << "handle Read\n";
	for (int i = 0; i < 101; i++)
		buf[i] = 0;
	r = recv(_soc.fd(), buf, 1024, 0);
	//std::cout << "r = " << r << ", buf = " << buf << std::endl;
	if (r <= 0)
	{
		std::cout << "client gone\n";
		return (true);
	}
	else if (r)
	{
		request.insert(buf, r);
		if (!request.gotFullMsg())
			return (false);
		request.execute(ms, this);
		request.reset();
		//reply("001");
	}
	return (false);
}

void	Session::reply(std::string const& str)
{
	std::string res = str;	
	res += "\r\n";
	send(_soc.fd(), res.c_str(), res.length(), 0);
}

Socket&	Session::soc() { return _soc; }
