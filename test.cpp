#include <iostream>
#include "Socket.hpp"

int			main(){
	Socket		soc(80);

	if (bind(soc.s(), (const sockaddr *)&(soc.sin()), sizeof(soc.sin()))){
		std::cout << "Bind Error" << std::endl;
		exit(0);
	}
	if (listen(soc.s(), 42) != 0){
		std::cout << "listen Error" << std::endl;
		exit(0);
	}
	//if (accept())
	int		ret;
	Socket	client(80);
	fd_set	rfds;
	fd_set	wfds;
	fd_set	except;
	struct timeval	tv;
	socklen_t	client_len;
	char	buf[1024];

	FD_ZERO(&rfds);
	FD_ZERO(&wfds);
	FD_ZERO(&except);
	FD_SET(0, &rfds);
	FD_SET(0, &wfds);
	FD_SET(0, &except);
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	while (1){
		client.s() = accept(soc.s(), (struct sockaddr*)&(client.sin()), &client_len);
		std::cout << "client_len = " << client_len << std::endl;
		if (client.s() == -1){
			std::cout << "Accept Error\n";
			exit(0);
		}
		while (1){
			memset(buf, 0, 50);
			if (read(client.s(), buf, 1024) <= 0){
				close(client.s());
				break;
			}
			std::cout << buf << std::endl;
			if (strncmp(buf, "quit", 4) == 0){
				write(client.s(), "bye", 3);
				close(client.s());
				exit(0);
			}
			write(client.s(), "end", 3);
			//std::cout << "send\n";
		}
		/*
		FD_CLR(0, &rfds);
		FD_CLR(0, &wfds);
		FD_CLR(0, &except);
		ret = select(42, &rfds, &wfds, &except, &tv);
		std::cout << "ret = " << ret << std::endl;
		if (ret == -2)
			std::cout << "Select Error\n";
		else if (ret){
			std::cout << "Data is available now.\n";
			if (rfds.fds_bits[0] != 0)
				std::cout << "rfds = " << rfds.fds_bits[0] << std::endl;
			if (wfds.fds_bits[0] != 0)
				std::cout << "wfds = " << wfds.fds_bits[0] << std::endl;
			if (except.fds_bits[0] != 0)
				std::cout << "except = " << except.fds_bits[0] << std::endl;
		}
		else
			std::cout << "No data within five seconds.\n";
		*/
		//if (i == 3)
		//	rfds.fds_bits[0] = 'A';
		//else if (i == 4)
		//	rfds.fds_bits[0] = 0;
	}
	close(client.s());
	close(soc.s());
	return 0;
}
