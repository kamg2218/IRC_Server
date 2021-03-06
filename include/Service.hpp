#ifndef SERVICE_HPP
# define SERVICE_HPP

#include <map>
#include <ctime>
#include <sys/resource.h>
#include <sys/select.h>
#include <sys/time.h>
#include "Session.hpp"
#include "MainServer.hpp"

class Service
{
	private:
		fd_set			_fdRead;
		fd_set			_fdWrite;
		int				_max;
		int				_res;
		struct timeval	_tv;
		void		sendPing(MainServer& sv, Session *ss);
	public:
		Service(void);
		Service(Service const& other);
		Service&	operator=(Service const& other);
		~Service(void);
		void		doSelect(MainServer const& sv);
		void		doService(MainServer & sv);
		class 		selectException : public std::exception
		{
			public:
				virtual const char *what(void) const throw();
		};
};

#endif
