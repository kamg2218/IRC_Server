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
		fd_set			fd_read;
		fd_set			fd_write;
		int				max;
		int				res;
		struct timeval	tv;
	public:
		Service();
		void	do_select(MainServer const& sv);
		void	do_service(MainServer & sv);
		void	sendPing(Session *ss);
		class SelectException : public std::exception
		{
			public:
				virtual const char *what(void) const throw()
				{
					return "Select Error\n";
				}
		};
};

#endif
