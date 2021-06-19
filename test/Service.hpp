#ifndef SERVICE_HPP
# define SERVICE_HPP

#include "Socket.hpp"
#include <map>
#include <exception>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
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
		Service(Socket& soc);
		Service(const Service& other);
		Service&	operator=(const Service& other);
		~Service();
		void		doSelect(Socket& soc, std::map<int, Socket>& mSockets);
		void		doService(Socket& soc, std::map<int, Socket>& mSockets);
		void		handleAccept(Socket& soc, std::map<int, Socket>& mSockets);
		void		set(Socket& soc, std::map<int, Socket>& mSockets);
		void		check_rfds(Socket& soc, std::map<int, Socket>& mSockets);
		//class		AcceptException;
		//class		SelectException;
		class		AcceptException : public std::exception
		{
			public:
				virtual const char* what(void) const throw()
				{
					return "Accept Error\n";
				}
		};

		class		SelectException : public std::exception
		{
			public:
				virtual const char* what(void) const throw()
				{
					return "Select Error\n";
				}
		};
};

#endif
