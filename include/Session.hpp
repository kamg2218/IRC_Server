#ifndef SESSION_HPP
# define SESSION_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <map>
#include <iostream>
#include "Executor.hpp"
class Session;
class User;
class Service;
#include "User.hpp"
#include "Service.hpp"
#include "Socket.hpp"

# define BUFSIZE 512

class Session
{
	private:
		typedef Session*	pointer;
		Socket			_soc;
		User			_user;
		bool			_ping;
		std::string		_rstream;
		Session(Session const& ref);
		Session&	operator=(Session const& ref);
	public:
		Session(void);
		Session(int sd);
		static pointer	create(void);
		int		socket(void) const;
		Socket&		soc(void);
		User&		user(void);
		void		setPing(bool p);
		bool		ping() const;
		bool		handleRead(std::map<int, Session*> & ms, int fd);
		void		streamAppend(char *str, int r);
		void		replyAsServer(std::string const& str);
		void		replyAsUser(Session *target, std::string const& str);
		void		rep001(User *us);
		void		rep311(Session *ss);
		void		rep313(Session *ss);
		void		rep315(std::string const& str);
		void		rep318(std::string const& nick);
		void		rep319(std::string const& str);
		void		rep321(void);
		void		rep322(Channel *ch);
		void		rep323(void);
		void		rep331(std::string const& chname);
		void		rep332(std::string const& chname, std::string const& topic);
		void		rep341(std::string const& chname, std::string const& nick);
		void		rep352(std::vector<std::string> const& v);
		void		rep353(std::string const& chname, std::string const& nic);
		void		rep366(std::string const& chname);
		void		rep381(void);
		void		err401(std::string const& nick);
		void		err402(std::string const& name);
		void		err403(std::string const& chname);
		void		err404(std::string const& chname);
		void		err406(std::string const& nick);
		void		err407(std::string const& nick);
		void		err411(std::string const& cmd);
		void		err412(void);
		void		err413(std::string const& mask);
		void		err414(std::string const& mask);
		void		err421(std::string const& cmd);
		void		err431(void);
		void		err432(std::string const& nick);
		void		err433(std::string const& nick);
		void		err442(std::string const& chanme);
		void		err443(std::string const& nick, std::string const& chname);
		void		err451(void);
		void		err461(std::string const& cmd);
		void		err462(void);
		void		err464(void);
		void		err482(std::string const& chname);
		void		err491(void);
};

#endif
