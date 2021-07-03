#include "include/Session.hpp"
#include "include/User.hpp"
#include "include/Service.hpp"

Session::Session()
	: _user(0), _server(0)
{
}

Session::Session(int csfd, Service* svc)
	: _user(0), _server(0)
{
	_soc.setSd(csfd);
}

Session::Session(Service* svc)
	: _user(0), _server(0)
{
}

Session::pointer	Session::create(Service* svc, int nbr)
{
	return (new Session(svc));
}

int		Session::socket() const
{
	return (_soc.sd());
}

bool	Session::handleRead(std::map<int, Session*> & ms, int sd)
{
	int		r;
	char	buf[1024];

	//std::cout << "handle Read\n";
	for (int i = 0; i < 101; i++)
		buf[i] = 0;
	r = recv(_soc.sd(), buf, 1024, 0);
	//std::cout << "r = " << r << ", buf = " << buf << std::endl;
	if (r <= 0)
	{
		std::cout << "client gone\n";
		return (true);
	}
	else if (r)
	{
		request.insert(buff, buf, r);
		if (!request.gotFullMsg(buff))
			return (false);
		request.execute(buff, ms, this);
		request.reset(buff);
		//reply("001");
	}
	return (false);
}

void	Session::reply(std::string const& str)
{
	std::string res = str;	
	res += "\r\n";
	send(_soc.sd(), res.c_str(), res.length(), 0);
}

Socket&	Session::soc() { return _soc; }
