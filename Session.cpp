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

void	Session::Rep_001(User *us){
	replyAsServer("001 :Welcome to the Internet Relay Network " + us->nick() + "!" + us->user() + "@" + us->host());
}
void	Session::Rep_321(){
	replyAsServer("321 Channel :Users Name");
}
void	Session::Rep_322(Channel *ch){
	replyAsServer("322 " + ch->name() + " " + std::to_string(ch->userCount()) + " :" + ch->topic());
}
void	Session::Rep_323(){
	replyAsServer("323 :End of /List");
}
void	Session::Rep_331(std::string const& ch){
	replyAsServer("331 " + ch + " :No topic is set");
}
void	Session::Rep_332(std::string const& ch, std::string const& topic){
	replyAsServer("332 " + ch + " :" + topic);
}
void	Session::Rep_341(std::string const& ch, std::string const& nick){
	replyAsServer("341 " + ch + " " + nick);
}
void	Session::Rep_353(std::string const& ch, std::string const& nick){
	replyAsServer("353 =" + ch + " :" + nick);
}
void	Session::Rep_366(std::string const& ch){
	replyAsServer("366 " + ch + " : End of NAMES list");
}
void	Session::Rep_381(){
	replyAsServer("381 :You are now an IRC operator");
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
	msg += " ";
	msg += chname;
	msg += " :No such channel";
	replyAsServer(msg);
}

void	Session::Err_404(std::string const& chname)
{
	std::string msg;
	
	msg += "404 ";
	msg += user().nick();
	msg += " ";
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
	msg += " ";
	msg += chname;
	msg += " :You;re not on that channel";
	replyAsServer(msg);
}

void	Session::Err_443(std::string const& nick, std::string const& chname)
{
	std::string msg;

	msg += "443 ";
	msg += user().nick();
	msg += " ";
	msg += nick;
	msg += " ";
	msg += chname;
	msg += " :is already on channel";
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
	msg += " ";
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

