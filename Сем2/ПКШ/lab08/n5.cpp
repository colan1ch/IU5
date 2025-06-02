#include <iostream>
#include <iomanip>


class Date{
    int day;
    int month;
    int year;
public:
    Date() : day(1), month(1), year(1970) {}

    Date(int d, int m, int y) : day(d), month(m), year(y) {}

    void print() {
        std::cout << std::setw(2) << std::setfill('0') << day << '.' << std::setw(2) << month << '.' << std::setw(4) << year << std::endl;
    }

    bool operator == (const Date &date) {
        return (day == date.day && month == date.month && year == date.year);
    }

    bool operator < (const Date &date) {
        if (year == date.year) {
            if (month == date.month) {
                return day < date.day;
            }
            return month < date.month;
        }
        return year < date.year;
    }
};


void print_vector(std::vector<Date> v) {
    for (Date i : v) i.print();
}

int main() {
    Date cur_date(21, 3, 2024);
    std::vector<Date> dates(6), new_dates;
    dates[0] = Date(1, 2, 1963);
    dates[1] = Date(14, 7, 1995);
    dates[2] = Date(7, 12, 2088);
    dates[3] = Date(5, 3, 2030);
    dates[4] = Date(24, 9, 2013);
    dates[5] = Date(19, 9, 2020);
    print_vector(dates);
    for (Date i : dates) {
        if (cur_date < i || cur_date == i) new_dates.push_back(i);
    }
    std::cout << std::endl;
    print_vector(new_dates);
    return 0;
}
