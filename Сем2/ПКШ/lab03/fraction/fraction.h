#pragma once
#include <iostream>


int gcd(int a, int b);


class Fraction {
    int numerator;
    int denumerator;
public:
    void simplify();

    Fraction();

    Fraction(int _numerator, int _denumerator);

    Fraction(const char *str);

    Fraction(const Fraction &fr);

    Fraction(double dbl);

    int getNumerator();

    void setNumerator(int _numerator);

    int getDenumerator() const;

    void setDenumerator(int _denumerator);

    Fraction operator + (const Fraction &fr);

    Fraction operator + (double dbl);

    Fraction operator + (int i);

    Fraction& operator += (const Fraction &fr);

    Fraction& operator += (double dbl);

    Fraction& operator += (int i);

    Fraction& operator = (double dbl);

    Fraction& operator = (const char *str);

    friend Fraction operator + (int i, const Fraction &fr);

    friend Fraction operator + (double dbl, const Fraction &fr);

    friend std::istream& operator >> (std::istream &in, Fraction &fr);

    friend std::ostream& operator << (std::ostream &out, const Fraction &fr);
};
