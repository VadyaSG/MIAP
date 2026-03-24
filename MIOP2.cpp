#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <numeric>
#include <iomanip>

void sum_vector(const std::vector<int>& v1, const std::vector<int>& v2, size_t start, size_t end, long long& result);

int main() 
{
    const int array_size = 1000000;
    std::vector<int> vec1(array_size);
    std::vector<int> vec2(array_size);

    for (int i = 0; i < array_size; ++i) 
    {
        vec1[i] = i + 1;
        vec2[i] = array_size - i;
    }

    unsigned int num_cores = std::thread::hardware_concurrency();
    std::cout << "Hardware cores: " << num_cores << std::endl;

    std::vector<int> test_sizes = { 1000, 10000, 100000, 1000000 };
    std::vector<int> thread_counts = { 1, 2, 4, 8, 16 };

    std::cout << std::setw(25) << "Threads";
    for (const auto& size : test_sizes) 
    {
        std::cout << std::setw(15) << size;
    }
    std::cout << std::endl;

    for (int num_threads : thread_counts) 
    {
        std::cout << std::setw(15) << num_threads << " Threads";

        for (int current_size : test_sizes) 
        {
            std::vector<int> current_vec1(current_size);
            std::vector<int> current_vec2(current_size);

            for (int i = 0; i < current_size; ++i) 
            {
                current_vec1[i] = i + 1;
                current_vec2[i] = current_size - i;
            }

            auto start_time = std::chrono::high_resolution_clock::now();

            std::vector<std::thread> threads;
            std::vector<long long> partial_sums(num_threads, 0);
            size_t chunk_size = current_size / num_threads;

            for (int i = 0; i < num_threads; ++i) 
            {
                size_t start = i * chunk_size;
                size_t end = (i == num_threads - 1) ? current_size : start + chunk_size;

                threads.emplace_back(sum_vector, std::ref(current_vec1), std::ref(current_vec2), start, end, std::ref(partial_sums[i]));
            }

            for (std::thread& t : threads) 
            {
                t.join();
            }

            long long total_sum = 0;

            for (long long sum : partial_sums)
            {
                total_sum += sum;
            }

            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end_time - start_time;

            std::cout << std::setw(15) << std::fixed << std::setprecision(7) << elapsed.count() << "s";
        }
        std::cout << std::endl;
    }

    return 0;
}

void sum_vector(const std::vector<int>& v1, const std::vector<int>& v2, size_t start, size_t end, long long& result)
{
    result = 0;
    for (size_t i = start; i < end; ++i) 
    {
        result += static_cast<long long>(v1[i]) + v2[i];
    }
}