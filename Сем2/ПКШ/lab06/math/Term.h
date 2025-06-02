#pragma once
#include <iostream>

class Polynomial;

class Term{
private:
    int multiplier;
    int power;
public:
    Term() : multiplier(0), power(0) {}

    Term(int _multiplier) : multiplier(_multiplier), power(0) {}

    Term(int _multiplier, int _power) : multiplier(_multiplier), power(_power) {}

    Term operator + (const Term &t) const;

    friend std::istream& operator >> (std::istream &in, Term &t);

    friend std::ostream& operator << (std::ostream &out, const Term &t);

    friend std::ostream& operator << (std::ostream &out, const Polynomial &p);

    friend std::istream& operator >> (std::istream &in, Polynomial &p);

    friend Polynomial operator * (const Polynomial &p1, const Polynomial &p2);

    friend Polynomial operator + (const Polynomial &p1, const Polynomial &p2);

    friend class Polynomial;
};
