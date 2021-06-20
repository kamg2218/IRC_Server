#include "Frame.hpp"

Frame::Frame()
{
	//sev.soc().makeSocket(80);
}

Frame::Frame(int port)
{
	//sev.soc().makeSocket(port);
	//if (sev.soc().port() != port)
	//	sev = Server(port);
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
	//close(sev.soc());
}
