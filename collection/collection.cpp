#include "collection_class.h"
#include <iostream>



void main(void)
{
	collection<int>* collection = new list<int>;
	collection->insert(*(collection->begin()), 1);
	auto begin = collection->begin();
	/*std::cout << **(collection->begin());
	std::cout << collection->begin().operator->()->operator*();*/

	for (size_t i = 0; i < 10; ++i)
	{
		collection->insert(*begin, i);
		begin = collection->begin();
	}

	begin = collection->begin();
	auto end = collection->end();
	while (*begin != *end)
	{
		std::cout << **begin;
		++*begin;
	}

}
