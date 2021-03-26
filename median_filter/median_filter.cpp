#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

std::vector<int> simple_median_filter(const std::vector<int> src, size_t w = 3)
{
	if (w > src.size())
		w = src.size();

	std::vector<int> dst;
	std::vector<int> window(w);

	auto count_add_number = w % 2==0 ? (w - 1) / 2 + 1 : (w - 1) / 2;

	for (size_t i = 0; i < count_add_number; ++i)
	{
		for(auto j= count_add_number-i;j>0;--j)
			window[j-1] = src[0];
		for (size_t k = 0; k < 2 + i; ++k)
			window[k + count_add_number-i] = src[k];
		std::sort(window.begin(), window.end());
		dst.push_back(window[w / 2]);
	}

	for (size_t i = w / 2; i <= src.size() - (w+1) / 2; ++i)
	{
		size_t k = 0;
		for (auto j = i - w / 2; j < i + (w+1) / 2; ++j)
		{
			window[k] = src[j];
			++k;
		}
		std::sort(window.begin(), window.end());
		dst.push_back(window[w / 2]);
	}

	auto size = src.size();
	if (w % 2 == 0)
		--count_add_number;
	for (size_t i = 0; i < count_add_number; ++i)
	{
		for (size_t k = w - 1 - i; k > 0; --k)
			window[w - 1 - k] = src[size - k];
		for (auto j = count_add_number - i; j > 0; --j)
			window[w-j] = src[size - 1];
		std::sort(window.begin(), window.end());
		dst.push_back(window[w / 2]);
	}
	return dst;
}

std::vector<int> median_filter(const std::vector<int> src, size_t w = 3)
{
	if (w > src.size())
		w = src.size();

	std::vector<int> dst;
	std::multiset<int> window;

	for (auto i = 0; i < w ; ++i)
		window.insert(src[i]);
	auto mid = window.begin();
	for (size_t i = 0; i < w / 2; ++i)
		++mid;
	dst.push_back(*mid);

	for (size_t i = w / 2 + 1; i <= src.size() - (w + 1) / 2; ++i)
	{
		auto tmp = window.find(src[i - w / 2 - 1]);
		bool equal_mid = false;
		if (*mid == *tmp)
		{
			++mid;
			equal_mid = true;
		}
		bool bigger_th_mid = false;
		if (*tmp > *mid)
			bigger_th_mid = true;

		//auto tmp = window.find(src[i - w / 2 - 1]);
		/*if (mid == tmp)
			++mid;*/
		window.erase(tmp);
		window.insert(src[i + (w+1) / 2 - 1]);
		if (src[i + (w + 1) / 2 - 1] < *mid && bigger_th_mid||equal_mid)
			--mid;
		if (src[i + (w + 1) / 2 - 1] > *mid && !bigger_th_mid)
			++mid;
		dst.push_back(*mid);
	}
	return dst;
}


void main(void)
{
	std::vector<int> v = { 2,2,56,3,4,2,4,5,123,14,1,2,3,4,1231,2,4,12,4 };
	std::vector<int> mfv1 = simple_median_filter(v,5);
	std::vector<int> mfv2 = median_filter(v, 5);
	for (auto& i : mfv1)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;
	std::cout << "  ";
	for (auto& i : mfv2)
	{
		std::cout << i << " ";
	}
}