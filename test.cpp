#include <iostream>
#include <vector>
#include <cctype>
int main()
{
	std::vector<std::string> res;
	std::string msgline = "nick jikwon user hi host hello\r\n";
	std::string::size_type pos;

	while ((pos = msgline.find(" ")) != std::string::npos)
	{
		res.push_back(msgline.substr(0, pos));
		msgline.erase(0, msgline.find(" ") + 1);
	}
	std::string::iterator temp = res[0].begin();
	while (temp != res[0].end())
	{
		*temp = toupper(*temp);
		++temp;
	}
	std::cout << "------\n";
	std::vector<std::string>::iterator it = res.begin();
	while (it != res.end())
	{
		std::cout << (*it) << std::endl;
		++it;
	}
	std::cout << "------\n";

}