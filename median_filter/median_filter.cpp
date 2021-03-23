#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

std::vector<int> simple_median_filter(const std::vector<int> src, size_t w = 3)
{
	std::vector<int> dst;
	std::vector<int> window(w);

	auto tmp = w % 2==0 ? (w - 1) / 2 + 1 : (w - 1) / 2;

	for (size_t i = 0; i < tmp; ++i)
	{
		for(auto j= tmp-i;j>0;--j)
			window[j-1] = src[0];
		for (size_t k = 0; k < 2 + i; ++k)
			window[k + tmp-i] = src[k];
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
		--tmp;
	for (size_t i = 0; i < tmp; ++i)
	{
		for (size_t k = w - 1 - i; k > 0; --k)
			window[w - 1 - k] = src[size - k];
		for (auto j = tmp - i; j > 0; --j)
			window[w-j] = src[size - 1];
		std::sort(window.begin(), window.end());
		dst.push_back(window[w / 2]);
	}
	return dst;
}

std::vector<int> median_filter(const std::vector<int> src, size_t w = 3)
{
	std::vector<int> dst;
	std::multiset<int> window;

	auto mid = window.begin();

	for (size_t i = w / 2; i <= src.size() - (w + 1) / 2; ++i)
	{
		for (auto j = i - w / 2; j < i + (w + 1) / 2; ++j)
			window.insert(src[j]);
		mid = window.begin();
		for (size_t j = 0; j < w / 2; ++j)
			++mid;
		dst.push_back(*mid);
		window.clear();
	}
	return dst;
}

void main(void)
{
	std::vector<int> v = { 2,2,56,3,4};
	std::vector<int> mfv1 = simple_median_filter(v, 3);
	std::vector<int> mfv2 = median_filter(v, 3);
	for (auto& i : mfv1)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;
	for (auto& i : mfv2)
	{
		std::cout << i << " ";
	}
}