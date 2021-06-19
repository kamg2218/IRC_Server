#include <iostream>
#include <memory>

int main()
{
	int a = 3;

	std::shared_ptr<int> p1(&a);
	std::cout << sizeof(p1) << std::endl;
	std::cout << "p1 : " << p1.use_count() << std::endl;
	std::shared_ptr<int> p2;
	p2 = p1;
	std::cout << "p1 : " << p1.use_count() << std::endl;
	std::cout << "p2 : " << p2.use_count() << std::endl;
	std::shared_ptr<int> p3;
	p3 = p1;
	std::cout << "p1 : " << p1.use_count() << std::endl;
	std::cout << "p2 : " << p2.use_count() << std::endl;
	std::cout << "p3 : " << p3.use_count() << std::endl;
	std::shared_ptr<int> p4;
	std::cout << "p4 : " << p4.use_count() << std::endl;
	std::shared_ptr<int> p5;
	std::cout << "p5 : " << p5.use_count() << std::endl;

}
