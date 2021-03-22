#include "find_common_words.h"
#include <algorithm>
#include <ctime>
#include <map>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>

using namespace std;

//vector<string> load_from_file(const char* path)
//{
//	auto f = fopen(path, "r");
//	vector<string> words;
//	char buffer[64] = { 0 };
//	while (fscanf(f, "%63s", buffer)!=EOF)
//	{
//		words.push_back(string(buffer));
//	}
//	return words;
//}

string& only_lower_char(string& str)
{
	for (auto it = str.begin(); it != str.end();)
	{
		if (*it > -1 && isalpha(*it))
		{
			*it = tolower(*it);
			++it;
		}
		else
		{
			str.erase(it);
			if (it == str.end())
				break;
		}
	}
	return str;
}

vector<string> load_from_file(const char* path)
{
	ifstream f(string(path), ios_base::in);
	vector<string> words;
	string tmp;

	if (!f)
		return words;

	while (f >> tmp)
		words.push_back(only_lower_char(tmp));

	return words;
}

size_t common_words_find(const vector<string>& words1, const vector <string>& words2)
{
	auto end1 = words1.end();
	auto end2 = words2.end();
	auto begin1 = words1.begin();
	auto begin2 = words2.begin();
	size_t count = 0;
	for (auto it = begin1; it!=end1; ++it)
	{
		auto first = find(begin1, it, *it);
		if (first == it)
			if (find(begin2, end2, *it) != end2)
				++count;
	}
	return count;
}

size_t common_words_lower_bound(const vector<string>& words1, const vector <string>& words2)
{
	size_t count = 0;
	vector<string> buf;
	auto begin1 = words1.begin();
	auto end1 = words1.end();
	auto begin2 = words2.begin();
	auto end2 = words2.end();
	for (auto it = begin1; it != end1; ++it)
	{
		auto tmp = lower_bound(buf.begin(), buf.end(), *it);
		if (tmp == buf.end())
		{
			buf.push_back(*it);
			continue;
		}
		if (*tmp == *it)
			continue;
		buf.insert(tmp, *it);
	}

	for (auto it = begin2; it != end2; ++it)
	{
		auto tmp = lower_bound(buf.begin(), buf.end(), *it);
		if (tmp == buf.end())
			continue;
		if (*tmp == *it)
		{
			++count;
			buf.erase(tmp);
		}
	}
	return count;
}

size_t common_words_one_set(const vector<string>& words1, const vector <string>& words2)
{
	size_t count = 0;
	unordered_set<string> set;
	auto begin2 = words2.begin();
	auto end2 = words2.end();
	for (auto& word:words1)
	{
		if (set.find(word) == set.end())
		{
			set.insert(word);
			if (find(begin2, end2, word) != end2)
				++count;
		}
	}
	return count;
}

size_t common_words_two_set(const vector<string>& words1, const vector <string>& words2)
{
	size_t count = 0;
	unordered_set<string> set1;
	unordered_set<string> set2;
	for (auto& word:words1)
		set1.insert(word);
	for (auto& word:words2)
	{
		if (set2.find(word) == set2.end())
		{
			set2.insert(word);
			if (set1.find(word) != set1.end())
				++count;
		}
	}
	return count;
}

bool compare_pair_second(const pair<string, size_t>& lhs, const pair<string, size_t>& rhs)
{
	return lhs.second < rhs.second;
}

map<string,size_t> find_common_repeat(const vector<string>& words1, const vector <string>& words2)
{
	unordered_map<string, size_t> map1;
	unordered_map<string, size_t> map2;
	map<string, size_t> result;
	for (auto& word : words1)
	{
		auto it = map1.find(word);
		if (it != map1.end())
			++(it->second);
		else
			map1.insert({ word, 1 });
	}

	for (auto& word : words2)
	{
		auto it = map2.find(word);
		if (it != map2.end())
			++(it->second);
		else
			map2.insert({ word, 1 });
	}

	for (auto& kv : map1)
	{
		auto it = map2.find(kv.first);
		if (it == map2.end())
			continue;
		if (it->second == kv.second)
			result.insert({ kv.first,kv.second });
	}
	return result;
}

int main()
{
	auto words1 = load_from_file("C:\\Users\\USER\\Desktop\\Учеба\\С С++\\CPP_practice\\find_common_words\\Sherlock-Holmes-by-Arthur-Conan-Doyle.txt");
	auto words2 = load_from_file("C:\\Users\\USER\\Desktop\\Учеба\\С С++\\CPP_practice\\find_common_words\\Tom-Sawyer-by-Mark-Twain.txt");
	auto start = clock();
	//auto count = common_words_two_set(words1, words2);
	auto map = find_common_repeat(words1, words2);

	//auto s = map.size();

	auto end = clock();
	for (auto& kv : map)
	{
		cout << kv.first << "   " << kv.second<<endl;
	}
	//cout << count<<endl<<(double)(end-start)/CLOCKS_PER_SEC;
}
