#ifndef SERVICE_HPP
# define SERVICE_HPP

#include "Socket.hpp"

class		Service
{
	private:
		int				max;
		struct timeval	tv;
		fd_set			rfds;
		fd_set			wfds;
		fd_set			except;
	public:
		Service();
		Service(const Service& other);
		Service&	operator=(const Service& other);
		~Service();
		void		doSelect(Socket& soc, std::map<int, Socket>& mSockets);
		void		doService(Socket& soc, std::map<int, Socket>& mSockets);
};

#endif
