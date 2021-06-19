#ifndef SERVICE_HPP
# define SERVICE_HPP

#include <exception>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>

class		Service
{
	private:
		int				max;
		int				ret;
		struct timeval	tv;
		fd_set			rfds;
		fd_set			wfds;
		fd_set			except;
	public:
		Service();
		Service(const Socket& soc);
		Service(const Service& other);
		Service&	operator=(const Service& other);
		~Service();
		void		doSelect(Socket& soc, std::map<int, Socket>& mSockets);
		void		doService(Socket& soc, std::map<int, Socket>& mSockets);
		void		handleAccept(Socket& soc, std::map<int, Socket>& mSockets);
		class		AcceptException();
		class		SelectException();
};

#endif
