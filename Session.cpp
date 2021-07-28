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
	r = recv(_soc.sd(), buf, bufsize, 0);

	if (r)
		StreamAppend(buf, r);
	while (request.gotFullMsg(rstream))
	{
		std::cout << "Got msg : " << request.getMessage(rstream) << std::endl;
		request.execute(rstream, this);
		request.reset(rstream);
	}
	if (r <= 0)
	{
		std::cout << "client gone\n";
		return (true);
	}
	return (false);

	/*
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
		StreamAppend(buf, r);
		while (request.gotFullMsg(rstream))
		{
			std::cout << "Got msg : " << request.getMessage(rstream) << std::endl;
			request.execute(rstream, this);
			request.reset(rstream);
		}
	}
	return (false);
	*/
}

//void	Session::replyAsServer(std::string const& str)
void	Session::replyAsServer(std::string const& str)
{
	std::string msg;

	msg += ":";
	msg += "server.hostnema "; //temp;
	msg += str;

	std::string res = str;	
	std::cout << "replied : " << str << "\n";
	res += "\n";
	send(_soc.sd(), res.c_str(), res.length(), 0);
}

void	Session::replyAsUser(Session *target, std::string const& str)
{
	std::string msg;

	msg += ":";
	msg += target->user().msgHeader();
	msg += str;
	send(target->soc().sd(), msg.c_str(), msg.length(), 0);
}

Socket&	Session::soc() { return _soc; }

User&	Session::user() { return _user; }

void	Session::Err_401(){}
void	Session::Err_402(){}
void	Session::Err_404(){}
void	Session::Err_406(){}
void	Session::Err_407(){}
void	Session::Err_411(){}
void	Session::Err_412(){}
void	Session::Err_413(){}
void	Session::Err_414(){}
void	Session::Err_431(){}
void	Session::Err_432(){}
void	Session::Err_433(){}
void	Session::Err_442(){}
void	Session::Err_443(){}
void	Session::Err_461(std::string const& str)
{
	std::string msg;

	msg += str;
	msg += " :Not enough parameters";
	replyAsServer(msg);
}

void	Session::Err_462(std::string const& str){}
void	Session::Err_464(std::string const& str){}
void	Session::Err_482(std::string const& str){}
void	Session::Err_491(std::string const& str){}
