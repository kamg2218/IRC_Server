#include "Frame.hpp"

int		main(void)
{
	try
	{
		Frame	mainFrame;
		mainFrame.start();
	}
	catch(std::exception const& err)
	{
		std::cout << err.what();
	}
}
