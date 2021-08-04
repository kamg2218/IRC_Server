#include <string>
#include <iostream>

void	function(std::vector<std::string> const& s)
{
	s[1] = "sdf";
}

int main()
{
	std::vector<std::string> s;

	s.push_back("hi");
	s.push_back("hello");
	function(s);
}
