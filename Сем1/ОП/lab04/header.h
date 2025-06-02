#pragma once
#include <iostream>
#include <cmath>
#include <iomanip>

double F(double val, int k);

double F_der(double val, int k);

void iterMethod(double eps, int k);

void binSearch(double eps, int k);

void NewtonMethod(double eps, int k);
