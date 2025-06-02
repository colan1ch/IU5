#include "Term.h"

class Polynomial{
private:
    Term *poly;
    int size;
    int degree;
    bool order_increase = false;
public:
    Polynomial() : poly(nullptr), degree(0), size(0) {}

    Polynomial(int multiplier);

    Polynomial(const Term &t);

    Polynomial(const Polynomial &p);

    ~Polynomial() { delete[] poly; }

    Polynomial& operator = (const Polynomial &p);

    Polynomial& operator += (const Polynomial &p);

    Polynomial& operator *= (const Polynomial &p);

    friend Polynomial operator * (const Polynomial &p1, const Polynomial &p2);

    friend Polynomial operator + (const Polynomial &p1, const Polynomial &p2);

    friend std::istream& operator >> (std::istream &in, Polynomial &p);

    friend std::ostream& operator << (std::ostream &out, const Polynomial &p);
};
