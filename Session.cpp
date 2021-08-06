#include "include/Session.hpp"
#include "include/User.hpp"
#include "include/Service.hpp"
#include "include/Frame.hpp"

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

void	Session::StreamAppend(char *str, int r)
{
	// 수정필요
	char ctrld = 4;

	for (int i = 0 ; i < r ; i++)
	{
		if (str[i] != ctrld)
			_rstream += str[i];
	}
}

bool	Session::handleRead(std::map<int, Session*> & ms, int sd)
{
	int		r;
	int		bufsize = 512;
	char	buf[512] = {0,};
	Executor	executor;

	r = recv(_soc.sd(), buf, bufsize, 0);

	if (r)
		StreamAppend(buf, r);
	while (executor.gotFullMsg(_rstream))
	{
		std::cout << "Got msg : " << executor.getMessage(_rstream) << std::endl;
		executor.execute(_rstream, this);
		executor.reset(_rstream);
	}
	if (r <= 0)
	{
		std::cout << "client gone\n";
		return (true);
	}
	return (false);
}

void	Session::Rep_352(std::vector<std::string> const& res)
{
	std::string msg;
	std::vector<std::string>::const_iterator it;

	for (it = res.begin(); it != res.end(); it++)
	{
		msg.clear();
		msg += "352 ";
		msg += user().nick();
		msg += " ";
		msg += (*it);
		replyAsServer(msg); // RPL_WHOREPLY
	}
}

void	Session::Rep_311(Session *ss)
{
	std::string msg;

	msg = "311 ";
	msg += user().nick();
	msg += " ";
	msg += ss->user().nick();
	msg += " ";
	msg += ss->user().nick();
	msg += " ";
	msg += ss->user().user();
	msg += " ";
	msg += ss->user().host();
	msg += " * :";
	msg += ss->user().name();
	replyAsServer(msg);
}

void	Session::Rep_313(Session *ss)
{
	std::string msg;

	msg = "313 ";
	msg += user().nick();
	msg += " ";
	msg += ss->user().nick();
	msg += " :is an IRC operator";
	replyAsServer(msg);
}

void	Session::Rep_318(std::string const& nick)
{
	std::string msg;

	msg = "318 ";
	msg += user().nick();
	msg += " ";
	msg += nick;
	msg += " :End of /WHOIS list";
	replyAsServer(msg);
}

void	Session::Rep_319(std::string const& str)
{
	std::string msg;

	msg = "319 ";
	msg += user().nick();
	msg += " ";
	msg += str;
	replyAsServer(msg);
}

void	Session::replyAsServer(std::string const& str)
{
	std::string msg;

	msg += ":";
	msg += Frame::instance()->GetServer().msgHeader() + " ";
	msg += str;
	std::cout << "replied as SV: " <<msg << "\n";
	msg += "\r\n";
	send(_soc.sd(), msg.c_str(), msg.length(), 0);
}

void	Session::replyAsUser(Session *target, std::string const& str)
{
	std::string msg;

	msg += user().msgHeader();
	msg += str;
	std::cout << "replied as US: " <<msg << "\n";
	msg += "\r\n";
	send(target->soc().sd(), msg.c_str(), msg.length(), 0);
}

Socket&	Session::soc() { return _soc; }

User&	Session::user() { return _user; }

void	Session::Rep_001(User *us)
{
	std::string	msg;

	msg = "001 ";
	msg += user().nick();
	msg += " :Welcome to the Internet Relay Network ";
	msg += us->nick();
	msg += "!";
	msg += us->user();
	msg += "@";
	msg += us->host();
	replyAsServer(msg);
}

void	Session::Rep_315(std::string const& str)
{
	std::string msg;

	msg = "315 ";
	msg += user().nick();
	msg += " ";
	msg += str;
	msg += " :End of /WHO list";
	replyAsServer(msg);
}

void	Session::Rep_321()
{
	std::string	msg;

	msg = "321 ";
	msg += user().nick();
	msg += " Channel :Users Name";
	replyAsServer(msg);
}

void	Session::Rep_322(Channel *ch)
{
	std::string	msg;

	msg = "322 ";
	msg += user().nick();
	msg += " #";
	msg += ch->name();
	msg += " ";
	msg += std::to_string(ch->userCount());
	msg += " :";
	msg += ch->topic();
	replyAsServer(msg);
}

void	Session::Rep_323()
{
	std::string	msg;

	msg = "323 ";
	msg += user().nick();
	msg += " :End of /List";
	replyAsServer(msg);
}

void	Session::Rep_331(std::string const& chname)
{
	std::string	msg;

	msg = "331 ";
	msg += user().nick();
	msg += " #";
	msg += chname;
	msg += " :No topic is set";
	replyAsServer(msg);
}

void	Session::Rep_332(std::string const& chname, std::string const& topic)
{
	std::string	msg;

	msg = "332 ";
	msg += user().nick();
	msg += " #";
	msg += chname;
	msg += " :";
	msg += topic;
	replyAsServer(msg);
}

void	Session::Rep_341(std::string const& chname, std::string const& nick)
{
	std::string	msg;

	msg = "341 ";
	msg += user().nick();
	msg += " #";
	msg += chname;
	msg += " ";
	msg += nick;
	replyAsServer(msg);
}

void	Session::Rep_353(std::string const& chname, std::string const& nick)
{
	std::string	msg;

	msg = "353 ";
	msg += user().nick();
	msg += " = #";
	msg += chname;
	msg += " :";
	msg += nick;
	replyAsServer(msg);
}

void	Session::Rep_366(std::string const& chname)
{
	std::string	msg;

	msg = "366 ";
	msg += user().nick();
	msg += " #";
	msg += chname;
	msg += " :End of NAMES list";
	replyAsServer(msg);
}

void	Session::Rep_381()
{
	std::string	msg;

	msg = "381 ";
	msg += user().nick();
	msg += " :You are now an IRC operator";
	replyAsServer(msg);
}

void	Session::Err_401(std::string const& nick)
{
	std::string msg;

	msg += "401 ";
	msg += user().nick();
	msg += " ";
	msg += nick;
	msg += " :No such nick/channel";
	replyAsServer(msg);
}

void	Session::Err_402(std::string const& name)
{
	std::string msg;

	msg += "402 ";
	msg += user().nick();
	msg += " ";
	msg += name;
	msg += " :No such server";
	replyAsServer(msg);
}

void	Session::Err_403(std::string const& chname)
{
	std::string msg;
	
	msg += "403 ";
	msg += user().nick();
	msg += " #";
	msg += chname;
	msg += " :No such channel";
	replyAsServer(msg);
}

void	Session::Err_404(std::string const& chname)
{
	std::string msg;
	
	msg += "404 ";
	msg += user().nick();
	msg += " #";
	msg += chname;
	msg += " :Cannot send to channel";
	replyAsServer(msg);
}

void	Session::Err_406(std::string const& nick)
{
	std::string msg;

	msg += "406 ";
	msg += user().nick();
	msg += " ";
	msg += nick;
	msg += " :There was no such nickname";
	replyAsServer(msg);
}

void	Session::Err_407(std::string const& nick)
{
	std::string msg;

	msg += "407 ";
	msg += user().nick();
	msg += " ";
	msg += nick;
	msg += " :Duplicate recipients. No message \\ delivered";
	replyAsServer(msg);
}

void	Session::Err_411(std::string const &cmd)
{
	std::string msg;

	msg += "411 ";
	msg += user().nick();
	msg += " ";
	msg += ":No recipient given";
	msg += cmd;
	replyAsServer(msg);
}

void	Session::Err_412(void)
{
	std::string msg;

	msg += "412 ";
	msg += user().nick();
	msg += " ";
	msg += ":No text to send";
	replyAsServer(msg);
}

void	Session::Err_413(std::string const& mask)
{
	std::string msg;

	msg += "413 ";
	msg += user().nick();
	msg += " ";
	msg += mask;
	msg += " :No toplevel domain specified";
	replyAsServer(msg);
}

void	Session::Err_414(std::string const& mask)
{
	std::string msg;
	
	msg += "414 ";
	msg += user().nick();
	msg += " ";
	msg += mask;
	msg += " :Wildcard in toplevel domain";
	replyAsServer(msg);
}

void	Session::Err_421(std::string const& cmd)
{
	std::string msg;

	msg += "421 ";
	msg += cmd;
	msg += " :Unknown command";
	replyAsServer(msg);
}

void	Session::Err_431(void)
{
	std::string msg;
	
	msg += "431 ";
	msg += user().nick();
	msg += " ";
	msg += ":No nickname given";
	replyAsServer(msg);
}

void	Session::Err_432(std::string const& nick)
{
	std::string msg;
	
	msg += "432 ";
	msg += user().nick();
	msg += " ";
	msg += nick;
	msg += " :Erroneus nickname";
	replyAsServer(msg);
}

void	Session::Err_433(std::string const& nick)
{
	std::string msg;

	msg += "433 ";
	msg += user().nick();
	msg += " ";
	msg += nick;
	msg += " :Nickname is already in use";
	replyAsServer(msg);
}

void	Session::Err_442(std::string const& chname)
{
	std::string msg;

	msg += "442 ";
	msg += user().nick();
	msg += " #";
	msg += chname;
	msg += " :You are not on that channel";
	replyAsServer(msg);
}

void	Session::Err_443(std::string const& nick, std::string const& chname)
{
	std::string msg;

	msg += "443 ";
	msg += user().nick();
	msg += " ";
	msg += nick;
	msg += " #";
	msg += chname;
	msg += " :is already on channel";
	replyAsServer(msg);
}

void	Session::Err_451(void)
{
	std::string msg;

	msg += "451 ";
	msg += ":You have not registered";
	replyAsServer(msg);
}

void	Session::Err_461(std::string const& cmd)
{
	std::string msg;

	msg += "461 ";
	msg += user().nick();
	msg += " ";
	msg += cmd;
	msg += " :Not enough parameters";
	replyAsServer(msg);
}

void	Session::Err_462(void)
{
	std::string msg;

	msg += "462 ";
	msg += user().nick();
	msg += " ";
	msg += ":You may not reregister";
	replyAsServer(msg);
}

void	Session::Err_464(void)
{
	std::string msg;

	msg += "464 ";
	msg += user().nick();
	msg += " ";
	msg += ":Password incorrect";
	replyAsServer(msg);
}

void	Session::Err_482(std::string const& chname)
{
	std::string msg;

	msg += "482 ";
	msg += user().nick();
	msg += " #";
	msg += chname;
	msg += " :You're not channel operator";
	replyAsServer(msg);
}

void	Session::Err_491(void)
{
	std::string msg;

	msg += "491 ";
	msg += user().nick();
	msg += " ";
	msg += ":No O-lines for your host";
	replyAsServer(msg);
}
