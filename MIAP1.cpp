#include<iostream>
#include<thread>
#include <chrono>
#include <windows.h>
#include <mutex>

void client(int max_client, int& client_counter, std::mutex& mtx);
void operacionist(int max_client, int& client_counter);
void SetColor(int text, int bg);

int main()
{
	int max_clients{ 20 };
	int client_counter{ 0 };
	std::mutex mtx;
	std::thread t1(client, max_clients, std::ref(client_counter), std::ref(mtx));
	std::thread t2(operacionist, max_clients, std::ref(client_counter));
	t1.join();
	t2.join();
}

void client(int max_client, int& client_counter)
{
	std::lock_guard<std::mutex> lock(mtx);
	for (int i = 0; i < max_client; ++i)
	{
		++client_counter;
		SetColor(4, 0);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << client_counter << " ";
	}
	std::lock_guard<std::mutex> unlock(mtx);
}

void operacionist(int max_client, int& client_counter)
{

	for (int i = max_client; i > 0; --i)
	{
		--client_counter;
		SetColor(15, 0);
		std::this_thread::sleep_for(std::chrono::seconds(2));
		std::cout << client_counter << " ";
	}
}

void SetColor(int text, int bg)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((bg << 4) | text));
}