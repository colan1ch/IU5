#include "math/Polynomial.h"

int main() {
    Term t1(1, 2), t2(1, 1);
    Polynomial p1(t1), p2(t2);
    p1 += p2;
    std::cout << p1 << '\n';
    p1 += Polynomial(Term(2, 2));
    p1 += Polynomial(Term(-2, 5));
    std::cout << p1 << '\n';
    Polynomial p3(Term(1, 1));
    p1 *= p3;
    std::cout << p1 << '\n';
    Term t(-5, 2);
    std::cout << t << '\n';

    Polynomial p;
    std::cin >> p;
    std::cout << p;
    return 0;
}
