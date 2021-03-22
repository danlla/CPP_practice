#include "avl_tree.h"


int main(void)
{
	set<int> s;

	for (int i = 1; i <= 5; ++i) {
		auto tmp = s.insert(i);
	}



	/*auto i = s.begin();

	while (i != s.end())
	{


		++i;


	}*/

	auto s2 = s;

	auto b = s.begin();

	auto c = s.end();


	while (c != s.begin())
	{


		--c;


	}


	s.debug();
	for (auto& x : s)
	{
		std::cout << x << std::endl;
	}

	auto check = s.contains(13);
}
