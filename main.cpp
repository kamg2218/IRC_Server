#include "Frame.hpp"

int		main(void)
{
	//Frame 서버
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
