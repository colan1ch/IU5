#include <iostream>
using namespace std;


void getSut(int h){
    if ((h >= 0) && (h < 5)) cout << "ночи";
    else if ((h >= 5) && (h < 12)) cout << "утра";
    else if ((h >= 12) && (h < 18)) cout << "дня";
    else cout << "вечера";
}


int main() {
    int minutes, hours24;
    do {
        cin >> hours24 >> minutes;
        int hours = hours24;
        if ((0 <= hours) && (hours < 24) && (minutes >= 0) && (minutes < 60)) {
            if (hours24 > 12) hours -= 12;
            if (hours24 == 0 && minutes == 0) { cout << "Полночь"; }
            else if (hours24 == 12 && minutes == 0) { cout << "Полдень"; }
            else {
                if (hours == 1) { cout << hours << " час "; }
                else if ((hours <= 4) && (hours != 0)) { cout << hours << " часа "; }
                else { cout << hours << " часов "; }
                if (minutes == 0) {
                    getSut(hours24);
                    cout << " ровно";
                } else {
                    if (((minutes % 10) >= 5) || ((minutes / 10 % 10) == 1) || (minutes % 10 == 0)) {
                        cout << minutes << " минут ";
                    } else if (minutes % 10 == 1) { cout << minutes << " минута "; }
                    else { cout << minutes << " минуты "; }
                    getSut(hours24);
                }
            }
        } else { cout << "Введены недопустимые данные"; }
    } while (true);
    return 0;
}
