#include "include/Session.hpp"
#include "include/User.hpp"
#include "include/Service.hpp"

Session::Session()
{
}

Session::Session(int sd)
{
	_soc.setSd(sd);
}

Session::pointer	Session::create()
{
	return (new Session());
}

int		Session::socket() const
{
	return (_soc.sd());
}

bool	Session::IsReady() const
{
	return (request.gotFullMsg(rstream));
}

void	Session::StreamAppend(char *str, int r)
{
	char ctrld = 4;

	for (int i = 0 ; i < r ; i++)
	{
		if (str[i] != ctrld)
			rstream += str[i];
	}
}

bool	Session::handleRead(std::map<int, Session*> & ms, int sd)
{
	int		r;
	int		bufsize = 512;
	char	buf[512] = {0,};
	//char	buf[bufsize];

	//std::cout << "handle Read\n";
	//for (int i = 0; i < bufsize; i++)
	//	buf[i] = 0;
	r = recv(_soc.sd(), buf, bufsize, 0);

	//std::cout << "r = " << r << ", buf = " << buf << std::endl;
	if (r <= 0)
	{
		std::cout << "client gone\n";
		// rstream 처리후에 종료:
		// QUIT 메시지 대신 작성해야함. 
		return (true);
	}
	else if (r)
	{
	//	rstream.append(buf, r);
	//	if (!request.gotFullMsg(rstream))
	//		return (false);
		//StreamAppend(buf, r);
		rstream.append(buf, r);
		while (request.gotFullMsg(rstream))
		{
			std::cout << "Got msg : " << request.getMessage(rstream) << std::endl;
			request.execute(rstream, this);
			request.reset(rstream);
		}
	}
	return (false);
}

void	Session::reply(std::string const& str)
{
	std::string res = str;	
	std::cout << "replied : " << str << "\n";
	res += "\n";
	send(_soc.sd(), res.c_str(), res.length(), 0);
}

Socket&	Session::soc() { return _soc; }

User&	Session::user() { return _user; }
