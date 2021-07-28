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

void	Session::Err_401(std::string const& nick)
{
	std::string msg;

	msg += nick;
	msg += " :No such nick/channel";
	replyAsServer(msg);
}

void	Session::Err_402(std::string const& name)
{
	std::string msg;

	msg += name;
	msg += " :No such server";
	replyAsServer(msg);
}

void	Session::Err_403(std::string const& chname)
{
	std::string msg;

	msg += chname;
	msg += " :No such channel";
	replyAsServer(msg);
}

void	Session::Err_404(std::string const& chname)
{
	std::string msg;

	msg += chname;
	msg += " :Cannot send to channel";
	replyAsServer(msg);
}

void	Session::Err_406(std::string const& nick)
{
	std::string msg;

	msg += nick;
	msg += " :There was no such nickname";
	replyAsServer(msg);
}

void	Session::Err_407(std::string const& nick)
{
	std::string msg;

	msg += nick;
	msg += " :Duplicate recipients. No message \\ delivered";
	replyAsServer(msg);
}

void	Session::Err_411(std::string const &cmd)
{
	std::string msg;

	msg += ":No recipient given";
	msg += cmd;
	replyAsServer(msg);
}

void	Session::Err_412(void)
{
	std::string msg;

	msg += ":No text to send";
	replyAsServer(msg);
}

void	Session::Err_413(std::string const& mask)
{
	std::string msg;

	msg += mask;
	msg += " :No toplevel domain specified";
	replyAsServer(msg);
}

void	Session::Err_414(std::string const& mask)
{
	std::string msg;

	msg += mask;
	msg += " :Wildcard in toplevel domain";
	replyAsServer(msg);
}

void	Session::Err_431(void)
{
	std::string msg;

	msg += ":No nickname given";
	replyAsServer(msg);
}

void	Session::Err_432(std::string const& nick)
{
	std::string msg;

	msg += nick;
	msg += " :Erroneus nickname";
	replyAsServer(msg);
}

void	Session::Err_433(std::string const& nick)
{
	std::string msg;

	msg += nick;
	msg += " :Nickname is already in use";
	replyAsServer(msg);
}

void	Session::Err_442(std::string const& chname)
{
	std::string msg;

	msg += chname;
	msg += " :You;re not on that channel";
	replyAsServer(msg);
}

void	Session::Err_443(std::string const& nick, std::string const& chname)
{
	std::string msg;

	msg += nick;
	msg += " ";
	msg += chname;
	msg += " :is already on channel";
	replyAsServer(msg);
}

void	Session::Err_461(std::string const& cmd)
{
	std::string msg;

	msg += cmd;
	msg += " :Not enough parameters";
	replyAsServer(msg);
}

void	Session::Err_462(void)
{
	std::string msg;

	msg += ":You may not reregister";
	replyAsServer(msg);
}

void	Session::Err_464(void)
{
	std::string msg;

	msg += ":Password incorrect";
	replyAsServer(msg);
}

void	Session::Err_482(std::string const& chname)
{
	std::string msg;

	msg += chname;
	msg += " :You're not channel operator";
	replyAsServer(msg);
}

void	Session::Err_491(void)
{
	std::string msg;

	msg += ":No O-lines for your host";
	replyAsServer(msg);
}

