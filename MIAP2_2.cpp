#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
#include <atomic>
#include <iomanip>
#include <sstream>

std::mutex mtx;
std::vector<std::string> output_lines;
char ch = 178;

void update(int threads)
{
	std::lock_guard<std::mutex> lock(mtx);
    std::cout << "\033[2J\033[H";
  
	for (int i = 0; i < threads; i++)
	{
		std::cout << output_lines[i] << std::endl;
	}
	std::cout.flush();
}
std::string createProgressBar(int width, char fill_сhar) 
{
    return std::string(width, fill_сhar);
}

void work(int thread_num, int duration_ms, int num_threads) 
{
    auto start = std::chrono::high_resolution_clock::now();

    {
        std::lock_guard<std::mutex> lock(mtx);
        std::stringstream ss;
        ss << "Thread " << thread_num << " [ID: " << std::this_thread::get_id() << createProgressBar(50, ' ') << "0%";
        output_lines[thread_num - 1] = ss.str();
    }
    update(num_threads);

    for (int i = 1; i <= 100; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(duration_ms / 100));
        {
            std::lock_guard<std::mutex> lock(mtx);
            std::stringstream ss;
            ss << "Thread: " << thread_num << " [ID: " << std::this_thread::get_id() << "] [";
            int pos = i / 2;
           
            for (int j = 0; j < 50; j++) 
            {
                if (j < pos)
                {
                    ss << ch;
                }
                else if (j == pos && i < 100)
                {
                    ss << "|";
                }
                else
                {
                    ss << " ";
                }
               
            }
            ss << "] " << i << "%";
            output_lines[thread_num - 1] = ss.str();
        }
        update(num_threads);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    {
        std::lock_guard<std::mutex> lock(mtx);
        std::stringstream ss;
        ss << "Thread " << thread_num << " [ID: " << std::this_thread::get_id() << "] "
           << createProgressBar(50, ch) << "100 % "
           << "Duration: " << duration.count() << " ms";
        output_lines[thread_num - 1] = ss.str();
    }
    update(num_threads);
}

int main()
{
    int num_threads = 8;
    int bar_length = 3000;

    output_lines.resize(num_threads);

    std::vector<std::thread> threads;

    std::cout << "Running " << num_threads << " Threads..." << std::endl;
    std::cout << "Bar length: " << bar_length << " ms" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << std::endl;

    for (int i = 1; i <= num_threads; i++) 
    {
        threads.emplace_back(work, i, bar_length, num_threads);
    }

    for (auto& t : threads) 
    {
        t.join();
    }

    std::cout << "\n----------------------------------------" << std::endl;
    std::cout << "All threads have completed their work " << std::endl;

}