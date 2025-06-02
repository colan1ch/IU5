#include <iostream>
#include <iomanip>
#include <list>


class Time{
    int second;
    int minute;
    int hour;
public:
    Time() : second(0), minute(0), hour(0) {}

    Time(int hr, int min, int sec) {
        min += sec / 60;
        sec %= 60;
        hr += min / 60;
        min %= 60;
        hr %= 24;
        second = sec;
        minute = min;
        hour = hr;
    }

    void print() {
        std::cout << std::setw(2) << std::setfill('0') << hour << ':' << std::setw(2) << minute << ':' << std::setw(2) << second << std::endl;
    }

    bool operator < (const Time &time) {
        if (hour == time.hour) {
            if (minute == time.minute) {
                return second < time.second;
            }
            return minute < time.minute;
        }
        return hour < time.hour;
    }

    bool operator == (const Time &time) {
        return (hour == time.hour && minute == time.minute && second == time.second);
    }
};


void print_list(std::list<Time> list) {
    for (auto i : list) {
        i.print();
    }
}


int main() {
    std::list<Time> time, night_time;
    time.push_back(Time(1, 2, 63));
    time.push_back(Time(14, 57, 19));
    time.push_back(Time(7, 32, 20));
    time.push_back(Time(5, 13, 23));
    time.push_back(Time(10, 19, 45));
    time.push_back(Time(19, 9, 59));
    print_list(time);
    Time night_st(0, 0, 0), night_end(5, 0, 0);
    for (auto i : time) {
        if (night_st < i && i < night_end) night_time.push_back(i);
    }
    std::cout << '\n';
    print_list(night_time);
    return 0;
}
