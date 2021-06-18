#include "Frame.hpp"

Frame::Frame()
{
}

Frame::Frame(int port)
{
	if (port != soc.port())
	{
		soc = Socket(port);
	}
	sev = Server(soc.s());
}

Frame::Frame(const Frame& other)
{
	*this = other;
}

Frame&	Frame::operator=(const Frame& other)
{
	if (this == &other)
		return *this;
	this->soc = other.soc;
	return *this;
}

void	Frame::start()
{
	sev.makeServer(soc);
	sev.runServer();
}

Frame::~Frame()
{
}
