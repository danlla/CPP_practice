#include "unordered_set_practice.h"

int main(void)
{
	unordered_set<int> a(20);
	a.insert(5);
	a.insert(6);
	a.insert(3);
	a.insert(7);
	a.insert(8);
	a.insert(10);
	a.insert(10);


	auto b = a;
	for (auto& x : b)
	{
		std::cout << x << std::endl;
	}
}
