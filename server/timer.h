#pragma once
#include <iostream>
#include <thread>
#include <atomic>
#include <functional>
#include <cstdio>
#include <chrono>
#include <ctime>

using namespace std;

class Timer {
public:
	Timer(int socket){
		this->socket = socket;
	}
	~Timer(){
		if (is_running){
			stop();
		}
	}
	
	typedef function<void(int)> Timeout;
	typedef chrono::milliseconds Interval;

	void start(int time_limit, const Interval &interval, const Timeout &timeout);
	void stop();

	private:
	int socket;
	atomic_bool is_running{};
	std::thread timer_thread{};

};
