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
	char	temp[512];

	//std::cout << "handle Read\n";
	for (int i = 0; i < 512; i++)
		temp[i] = '\0';
	r = recv(_soc.sd(), temp, 512, 0);
	//std::cout << "r = " << r << ", buf = " << buf << std::endl;
	if (r <= 0)
	{
		std::cout << "client gone\n";
		return (true);
	}
	else if (r)
	{
		//buff += temp; // r 만큼만 해야함. 
		insert(temp, r);
		if (!gotFullMsg())
			return  (false);
		std::cout << "client " << _soc.sd() << " : ";
		std::cout << buff.substr(0, buff.find("\r\n")) << std::endl;
		reset();
		/*
		request.insert(buff, buf, r);
		if (!request.gotFullMsg(buff))
			return (false);
		request.execute(buff, ms, this);
		request.reset(buff);
		*/
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

bool	Session::gotFullMsg()
{
	std::string::size_type	res;

	res = buff.find("\r\n");
	if (res == std::string::npos)
		return (false);
	return (true);
}

void	Session::reset()
{
	buff.erase(0, buff.find("\r\n") + 2);
}

void	Session::insert(char *str, int r)
{
	for (int i = 0 ; i < r ; i++)
	{
		buff += str[i];
		str[i] = '\0';
	}
}

Socket&	Session::soc() { return _soc; }
