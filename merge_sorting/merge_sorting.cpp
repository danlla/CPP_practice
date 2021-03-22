#include "merge_sorting.h"
#include <vector>

bool comparator(int first, int second)
{
	return first <= second;
}

int main(void)
{
	std::vector<int> v = { 7,8,4,5,3,11,15,14,9,7,10,1};
	merge_sort(v, comparator);
	for (auto& x : v)
	{
		std::cout << x << std::endl;
	}
}
