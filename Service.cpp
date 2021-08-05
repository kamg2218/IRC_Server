#include "include/Service.hpp"
#include "include/User.hpp"
#include "include/Frame.hpp"

Service::Service()
{
	struct rlimit rlp;
	
	getrlimit(RLIMIT_NOFILE, &rlp);
	maxopen = rlp.rlim_cur;
}

void	Service::do_select(MainServer const& sv)
{
	res = 0;
	FD_ZERO(&fd_read);
	FD_SET(sv.socket(), &fd_read);
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	max = sv.socket();
	for (std::map<int, Session*>::const_iterator it = sv.users().begin() ; it != sv.users().end() ; ++it)
	{
		FD_SET(it->first, &fd_read);
		max = std::max(max, it->first);
	}
	res = select(max + 1, &fd_read, NULL, NULL, &tv);
	if (res == -1)
		throw SelectException();
}

void	Service::do_service(MainServer & sv)
{
	if (res <= 0)
		return ;
	if (FD_ISSET(sv.socket(), &fd_read))
	{
		sv.handleAccept(this);
		std::cout << "client is accepted\n";
	}
	for (std::map<int, Session*>::iterator it = sv.users().begin(); it != sv.users().end() ; )
	{
		std::map<int, Session*>::iterator temp = it++;
		if (FD_ISSET(temp->first, &fd_read))
			sv.handleRead(temp);
		else
			sendPing(temp->second);
	}
}

void	Service::sendPing(Session *ss)
{
	std::string	msg;
	std::vector<std::string>	v;

	if (ss->ping() == false)
	{
		v.insert(v.end(),"QUIT");
		v.insert(v.end(), ":" + ss->soc().sd() + " client is missing");
		Frame::instance()->cmdQuit(ss, v);
		return ;
	}
	msg = "PING ";
	msg += ss->user().nick();
	msg += "\r\n";
	send(ss->soc().sd(), msg.c_str(), msg.length(), 0);
	ss->setPing(false);
}

int		Service::getMaxopen() const
{
	return (maxopen);
}
