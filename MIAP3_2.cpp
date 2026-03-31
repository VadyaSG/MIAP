#include <vector>
#include <thread>
#include <future>
#include <numeric>
#include <iostream>

template <typename T,typename T2>
void par_for_each(T begin, T end, T2 func);

void print(int val);

int main()
{
	std::vector<int> vec(10000);
	std::iota(vec.begin(), vec.end(), 0);
	par_for_each(vec.begin(), vec.end(), print);
	return 0;
}

template <typename T, typename T2>
void par_for_each(T begin, T end, T2 func)
{
	auto size = std::distance(begin, end);

	if (size < 1000)
	{
		for (auto it = begin; it != end; ++it)
		{
			func(*it);
		}
		return;
	}

	auto mid = std::next(begin, size / 2);

	auto future = std::async(std::launch::async, [&] {par_for_each(begin, mid, func); });
	par_for_each(mid, end, func);
	future.get();
}

void print(int val)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	std::cout << val << " ";
}
