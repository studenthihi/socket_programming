#include"timer.h"

void Timer::start(int time_limit, const Interval &interval, const Timeout &timeout){
    is_running = true;
    timer_thread = std::thread([this, time_limit, interval, timeout](){
        int count = 0;
        while (is_running){
            // cout << count << endl;
            this_thread::sleep_for(interval);
            count += 1;
            if (count >= time_limit) {
                timeout(socket);
                break;
            }
        }
    });
}
void Timer::stop(){
    // cout << "Timer stopped" << endl;
    is_running = false;
    timer_thread.join();
}