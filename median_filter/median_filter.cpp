#include <iostream>
#include <vector>
#include <algorithm>

std::vector<int> simple_median_filter(const std::vector<int> src, size_t w)
{
	std::vector<int> dst;
	std::vector<int> window(w);

	window[0] = src[0];
	for (size_t i = 0; i < w-1; ++i)
		window[i + 1] = src[i];
	std::sort(window.begin(), window.end());
	dst.push_back(window[w / 2]);

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
	return dst;
}

void main(void)
{
	std::vector<int> v = { 2,2,56,3,4};
	std::vector<int> mfv = simple_median_filter(v, 4);
	for (auto& i : mfv)
	{
		std::cout << i << " ";
	}
}