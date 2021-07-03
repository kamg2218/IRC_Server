#ifndef FT_SERVICE_HPP
# define FT_SERVICE_HPP

#include <map>
#include <sys/resource.h>
#include <sys/select.h>
#include <sys/time.h>
#include "Session.hpp"
#include "MainServer.hpp"

class Service
{
	private:
		std::map<int, Session*>	mSessions;
		fd_set			fd_read;
		fd_set			fd_write;
		int				max;
		int				maxopen;
		int				res;
		struct timeval	tv;

	public:
		Service();
		void	do_set();
		void	do_select(MainServer const& sv);
		void	do_service(MainServer & sv);
		int		getMaxopen() const;
		std::map<int, Session*>&	users() { return mSessions; }
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
