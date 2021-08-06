#ifndef SERVICE_HPP
# define SERVICE_HPP

#include <map>
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
	public:
		Service();
		Service(const Service& other);
		Service&	operator=(const Service& other);
		~Service();
		void		doSelect(MainServer const& sv);
		void		doService(MainServer & sv);
		void		sendPing(Session *ss);
		class 		selectException : public std::exception
		{
			public:
				virtual const char *what(void) const throw();
		};
};

#endif
