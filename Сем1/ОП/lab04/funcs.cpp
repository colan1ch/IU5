#include "header.h"
using namespace std;


double F(double val, int k){
    return val - k * cos(val);
}


double F_der(double val, int k){
    return 1 + k * sin(val);
}


void iterMethod(double eps, int k){
    int count = 0;
    double x = 0;
    while ((abs(F(x, k)) >= eps) and (count < 100000000)){
        x -= F(x, k);
        count++;
    }
    cout << "Метод итераций\n";
    cout << "Ответ: " << fixed << setprecision(8) << x << '\n';
    cout << "Количество итераций: " << count << "\n\n";
}


void binSearch(double eps, int k){
    int count = 0;
    double l = 0, r = k, x = k;
    while (abs(F(x, k)) > eps){
        count++;
        x = (r + l) / 2;
        if (F(l, k) * F(x, k) < 0){
            r = x;
        }
        else{
            l = x;
        }
    }
    cout << "Метод половинного деления\n";
    cout << "Ответ: " << fixed << setprecision(8) << x << '\n';
    cout << "Количество итераций: " << count << "\n\n";
}


void NewtonMethod(double eps, int k){
    double x = 1;
    int count = 0;
    while (abs(F(x, k)) > eps){
        x = x - (F(x, k)) / F_der(x, k);
        count++;
    }
    cout << "Метод Ньютона\n";
    cout << "Ответ: " << fixed << setprecision(8) << x << '\n';
    cout << "Количество итераций: " << count << "\n\n";
}
