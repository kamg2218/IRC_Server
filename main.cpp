#include <iostream>
#include "Socket.hpp"

struct		s_fd{
	int				max_fd;
	struct timeval	tv;
	fd_set			rfds;
	fd_set			wfds;
	fd_set			except;
};

void		set_fd(s_fd* fd){
	FD_ZERO(&fd->rfds);
	FD_ZERO(&fd->wfds);
	FD_ZERO(&fd->except);
	FD_SET(soc.s(), &fd->rfds);
	FD_SET(soc.s(), &fd->wfds);
	FD_SET(soc.s(), &fd->except);
	fd->tv.tv_sec = 5;
	fd->tv.tv_usec = 0;
	fd->max_fd = soc.s();
}

int			main(){
	int			ret;
	Socket		soc(80);
	Socket		client(80);
	socklen_t	client_len;
	char		buf[1024];
	s_fd		fd;

	if (bind(soc.s(), (const sockaddr *)&(soc.sin()), sizeof(soc.sin()))){
		std::cout << "Bind Error" << std::endl;
		exit(0);
	}
	if (listen(soc.s(), 42) != 0){
		std::cout << "listen Error" << std::endl;
		exit(0);
	}
	std::cout << "max = " << fd.max_fd << std::endl;
	while (1){
		ret = select(fd.max_fd + 1, &fd.rfds, &fd.wfds, &fd.except, &fd.tv);
		std::cout << "ret = " << ret << std::endl;
		if (ret == -1)
			std::cout << "Select Error\n";
		else if (ret){
			for (int i = 0; i < fd.max_fd + 1; i++){
				if (FD_ISSET(i, &fd.rfds)){
					if (i == soc.s()){
						client.s() = accept(soc.s(), (struct sockaddr*)&(client.sin()), &client_len);
						std::cout << "client_len = " << client_len << std::endl;
						if (client.s() == -1){
							std::cout << "Accept Error\n";
							exit(0);
						}
						FD_SET(client.s(), &fd.rfds);
						if (fd.max_fd < client.s())
							fd.max_fd = client.s();
					}
					else{
						if (read(i, buf, 1024) <= 0){
							std::cout << "Read Error\n";
							exit(0);
						}
						//buf[strlen(buf) - 1] = 0;
						std::cout << "message = " << buf << std::endl;
						write(client.s(), "end", 1024);
						if (strncmp(buf, "quit", 4) == 0){
							close(client.s());
							close(soc.s());
							exit(1);
						}
						memset(buf, 0, 1024);
					}
				}
			}
		}
		else
			std::cout << "No data within five seconds.\n";
	}
	close(client.s());
	close(soc.s());
	return 0;
}
