#ifndef FT_SERVICE_HPP
# define FT_SERVICE_HPP

#include <map>
#include <sys/resource.h>
#include <sys/select.h>
#include "Server.hpp"
#include "Session.hpp"

class Service
{
	private:
		std::map<int, Session*>	mSessions;
		fd_set	fd_read;
		fd_set	fd_write;
		int		max;
		int		maxopen;
		int		res;

	public:
		Service();
		void	do_select(Server const& sv);
		void	do_service(Server & sv);
		int		getMaxopen() const;
};

#endif
