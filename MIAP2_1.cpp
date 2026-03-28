#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>
#include <atomic> 

void SetColor(int text, int bg);
void client(int max_client, std::atomic<int>& client_counter);
void operacionist(int max_client, std::atomic<int>& client_counter);
    
int main() 
{
    int max_clients{ 20 };
    std::atomic<int> client_counter{ 0 };

    std::thread t1(client, max_clients, std::ref(client_counter));
    std::thread t2(operacionist, max_clients, std::ref(client_counter));

    t1.join();
    t2.join();
    return 0;
}

void SetColor(int text, int bg)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, (WORD)((bg << 4) | text));
}

void client(int max_client, std::atomic<int>& client_counter)
{
    for (int i = 0; i < max_client; ++i) 
    {
        int current_val = client_counter.fetch_add(1) + 1;
        SetColor(4, 0);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << current_val << " ";
    }
}

void operacionist(int max_client, std::atomic<int>& client_counter) 
{
    for (int i = 0; i < max_client; ++i) 
    {
        int current_val = client_counter.fetch_sub(1) - 1;
        SetColor(15, 0);
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << current_val << " ";
    }
}