#pragma once
#include <iomanip>
#include <iostream>
#include <cmath>
using namespace std;


typedef double (*TPF) (double);


struct I_Print{
    char* name;
    double i_sum;
    double i_toch;
    int n;
};

void PrintTabl(I_Print i_prn[],int k);

double f1(double x);

double f2(double x);

double f3(double x);

double f4(double x);

double IntRect(TPF f, double a, double b, double eps, int& n);

double IntTrap(TPF f, double a, double b, double eps, int& n);
