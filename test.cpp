#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <cstring>

int main()
{
	std::string str = "1423";

	std::vector<std::string> v = split(str, ":");
	
	std::vector<std::string>::iterator it = v.begin();
	std::cout << "size : " << v.size() << std::endl;
	while (it != v.end())
	{
		std::cout << *it << std::endl;
		it++;
	}

	return 0;
}
