#include "Frame.hpp"

Frame::Frame()
{
	svi = Service(sev.soc());
}

Frame::Frame(int port)
{
	if (port != sev.soc().port())
	{
		sev = Server(port);
		svi = Service(sev.soc());
	}
}

Frame::Frame(const Frame& other)
{
	*this = other;
}

Frame&	Frame::operator=(const Frame& other)
{
	if (this == &other)
		return *this;
	this->sev = other.sev;
	this->svi = other.svi;
	this->mSockets = other.mSockets;
	return *this;
}

void	Frame::start()
{
	sev.makeServer(sev.soc());
	while (1)
	{
		svi.doSelect(sev.soc(), mSockets);
	}
}

Frame::~Frame()
{
}
