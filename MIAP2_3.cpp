#include <iostream>
#include <mutex>
#include <thread>
#include <algorithm>

class Data
{
public:
	int val;
	std::mutex mtx;
	Data(int val) :val(val) {}
};

void unique_lock_swap(Data& a, Data& b)
{
	std::cout << "Unique: " << std::endl;
	std::unique_lock<std::mutex>lock_a(a.mtx, std::defer_lock);
	std::unique_lock<std::mutex>lock_b(b.mtx, std::defer_lock);
	std::cout << "swap "<<a.val<<" and "<<b.val<<std::endl;
	std::swap(a.val, b.val);
	std::cout << "Swaped " << a.val << " " << b.val<<"\n" << std::endl;
}

void scoped_lock_swap(Data& a, Data& b)
{
	std::cout << "Scoped: " << std::endl;
	std::scoped_lock lock(a.mtx, b.mtx);
	std::cout << "swap " << a.val << " and " << b.val << std::endl;
	std::swap(a.val, b.val);
	std::cout << "Swaped " << a.val << " " << b.val << "\n" << std::endl;
}

void just_lock_swap(Data& a, Data& b)
{
	std::cout << "lock: " << std::endl;
	std::lock(a.mtx, b.mtx);
	std::lock_guard<std::mutex> lock_a(a.mtx, std::adopt_lock);
	std::lock_guard<std::mutex> lock_b(b.mtx, std::adopt_lock);
	std::cout << "swap " << a.val << " and " << b.val << std::endl;
	std::swap(a.val, b.val);
	std::cout << "Swaped " << a.val << " " << b.val << std::endl;
}

int main()
{
	Data obj1(5);
	Data obj2(8);
	
	unique_lock_swap(obj1, obj2);
	
	scoped_lock_swap(obj1, obj2);
	
	just_lock_swap(obj1, obj2);
}