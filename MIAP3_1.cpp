#include <iostream>
#include <vector>
#include <future>
#include <thread>
#include <algorithm>

void find_min_element(std::vector<int>::iterator begin, std::vector<int>::iterator end, std::promise<std::vector<int>::iterator> promise);

void selection_sort_async(std::vector<int>& vec);

void printVector(const std::vector<int>& vec);

int main() 
{
    std::vector<int> vec = { 64, 25, 12, 22, 11, 99, 5, 38 };

    std::cout << "start vector: ";
    printVector(vec);

    selection_sort_async(vec);

    std::cout << "sorted vector: ";
    printVector(vec);

    return 0;
}

void find_min_element(std::vector<int>::iterator begin, std::vector<int>::iterator end, std::promise<std::vector<int>::iterator> promise)
{
    if (begin == end)
    {
        promise.set_value(end);
        return;
    }

    auto min = begin;

    for (auto it = std::next(begin); it != end; ++it) 
    {
        if (*it < *min) 
        {
            min = it;
        }
    }
    promise.set_value(min);
}

void selection_sort_async(std::vector<int>& vec)
{
    int n = vec.size();

    for (int i = 0; i < n - 1; ++i)
    {
        std::promise<std::vector<int>::iterator> min_promise;
        std::future<std::vector<int>::iterator> min_future = min_promise.get_future();
        std::thread workerThread(find_min_element, vec.begin() + i, vec.end(), std::move(min_promise));
        std::vector<int>::iterator min = min_future.get();

        if (min != vec.begin() + i)
        {
            std::swap(vec[i], *min);
        }
        workerThread.join();
    }
}

void printVector(const std::vector<int>& vec)
{
    for (int x : vec)
    {
        std::cout << x << " ";
    }
    std::cout << std::endl;  
}