

template <typename It, typename Comp>
void merge(It first1, It last1, It first2, It last2, It& first_res, It& last_res, bool direction, Comp comparator)
{
	++last1;
	--first2;
 	while (true)
	{
		if (first1 != last1 && first2 != last2)
		{
			if (comparator(*first1, *last2))
			{
				if (direction)
				{
					*first_res = *first1;
					++first_res;
				}
				else
				{
					*last_res = *first1;
					--last_res;
				}
				++first1;
			}
			else
			{
				if (direction)
				{
					*first_res = *last2;
					++first_res;
				}
				else
				{
					*last_res = *last2;
					--last_res;
				}
				--last2;
			}
		}

		if (first1 == last1 && first2 != last2)
		{
			if (direction)
			{
				*first_res = *last2;
				++first_res;
			}
			else
			{
				*last_res = *last2;
				--last_res;
			}
			--last2;
		}

		if (first1 != last1 && first2 == last2)
		{
			if (direction)
			{
				*first_res = *first1;
				++first_res;
			}
			else
			{
				*last_res = *first1;
				--last_res;
			}
			++first1;
		}

		if (first1 == last1 && first2 == last2)
			break;
	}
}

template <typename T,typename Comp>
void merge_sort(T& data, Comp comparator)
{
	bool finish = 0;
	T buf(data.size());
	auto i_src = data.begin();
	auto j_src = --data.end();
	auto i_src_cur = i_src;
	auto j_src_cur = j_src;
	auto i_buf = buf.begin();
	auto j_buf = --buf.end();
	bool even = 1;
	while (true)
	{
		while (comparator(*i_src_cur, *(i_src_cur + 1)) && i_src_cur < j_src)
			++i_src_cur;
		while (comparator(*j_src_cur, *(j_src_cur - 1)) && i_src_cur < j_src_cur)
			--j_src_cur;
		if (i_src_cur >= j_src_cur)
		{
			++j_src_cur;
			finish = 1;
		}
		merge(i_src, i_src_cur, j_src_cur, j_src, i_buf, j_buf, 1, comparator);
		if (finish)
			break;

		auto i2 = ++i_src_cur;
		auto j2 = --j_src_cur;
		while (comparator(*i_src_cur, *(i_src_cur + 1)))
			++i_src_cur;
		while (comparator(*j_src_cur, *(j_src_cur - 1)) && i_src_cur < j_src_cur)
			--j_src_cur;
		if (i_src_cur >= j_src_cur)
		{
			++j_src_cur;
		}
		merge(i2, i_src_cur, j_src_cur, j2, i_buf, j_buf, 0, comparator);

		/*for (auto &x : buf)
		{
			std::cout << x << std::endl;
		}
		std::cout << "\n\n";
		for (auto &x : data)
		{
			std::cout << x << std::endl;
		}
		std::cout << "\n\n";*/

		i_src = i_buf;
		j_src = j_buf;
		++i_src_cur;
		while (i_src_cur <= j_src_cur)
		{
			*i_src = *i_src_cur;
			++i_src;
			++i_src_cur;
		}

		if (even)
		{
			i_src_cur = i_src = buf.begin();
			j_src_cur = j_src = --buf.end();
			i_buf = data.begin();
			j_buf = --data.end();
		}
		else
		{
			i_src_cur = i_src = data.begin();
			j_src_cur = j_src = --data.end();
			i_buf = buf.begin();
			j_buf = --buf.end();
		}
		even = !even;
	}

	if (even)
	{
		i_src = data.begin();
		for (auto& x : buf)
		{
			*i_src = x;
			++i_src;
		}
	}
}