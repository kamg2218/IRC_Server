#include "include/Frame.hpp"

int main()
{
	try
	{
		Frame *mainframe = Frame::instance();
		mainframe->start();
	}
	catch(std::exception const& err)
	{
		std::cout << err.what() << "\n";
	}
}
