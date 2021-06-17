#ifndef FT_FD_HPP
# define FT_FD_HPP

class Fd
{	
	public:
	enum HostType
	{
		SERVER,
		CLIENT,
		FREE
	};

	private:
	HostType	type;
	User		*user;
	Server		*server;
	std::string		buf_read;
	std::string		buf_write;

	public:
	Fd() : type(FREE), user(NULL), server(NULL)
	{}

	void set(User *p)
	{
		type = CLIENT;
		user = p;
		server = NULL;
	}
	void set(Server *p)
	{
		type = SERVER;
		server = p;
		user = NULL;
	}
	/*
	void setType(HostType tp)
	{
		if (type == FREE)
			type = tp;
	}
	*/
	HostType getType() const
	{
		return (type);
	}
	void clean()
	{
		type = FREE;
	}
	std::string& rbuf()
	{
		return (buf_read);
	}
	std::string& wbuf()
	{
		return (buf_write);
	}
	void setFctRead(int fd);
	void setFctWrite(int fd);
	std::string& BufRead();
	std::string& BufWrite();
	
};

#endif
