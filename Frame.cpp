#include "Frame.hpp"

Frame::Frame()
{
}

Frame::Frame(int port)
{
	if (sev.soc().port() != port)
		sev = Server(port);
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
	sev.makeServer();
	while (1)
	{
		svi.set(sev.soc(), mSockets);
		svi.doSelect(sev.soc(), mSockets);
	}
}

Frame::~Frame()
{
}
