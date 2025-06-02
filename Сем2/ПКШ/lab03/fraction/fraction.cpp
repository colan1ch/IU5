#include "fraction.h"

int gcd(int a, int b) {
    return b ? gcd(b, a % b) : a;
}


void Fraction::simplify() {
    int _gcd = gcd(numerator, denumerator);
    numerator /= _gcd;
    denumerator /= _gcd;
    if (denumerator < 0) {
        numerator *= -1;
        denumerator *= -1;
    }
}


Fraction::Fraction() {
    numerator = 0;
    denumerator = 1;
}

Fraction::Fraction(int _numerator, int _denumerator) {
    if (_denumerator == 0) {
        std::cout << "Dividing by zero\n";
        exit(0);
    }
    numerator = _numerator;
    denumerator = _denumerator;
    simplify();
}

Fraction::Fraction(const char *str) {
    int int_part = 0, num = 0, denum = 1, len = strlen(str);
    int space_ind = -1, slash_ind = -1;
    for (int i = 0; i < len - 1; i++) {
        if (str[i] == ' ' && str[i + 1] != ' ') {
            space_ind = i;
            char int_part_a[i + 1];
            for (int j = 0; j < i; j++) {
                int_part_a[j] = str[j];
            }
            int_part_a[i] = '\0';
            int_part = atoi(int_part_a);
            break;
        }
    }
    for (int i = space_ind + 1; i < len; i++) {
        if (str[i] == '/') {
            slash_ind = i;
            char num_a[len - space_ind + 1], denum_a[len - space_ind + 1];
            for (int j = space_ind + 1; j < i; j++) {
                num_a[j - space_ind - 1] = str[j];
            }
            num_a[i - space_ind - 1] = '\0';
            for (int j = i + 1; j < len; j++) {
                denum_a[j - i - 1] = str[j];
            }
            denum_a[len - i - 1] = '\0';
            num = atoi(num_a);
            denum = atoi(denum_a);
            break;
        }
    }
    if (slash_ind == -1) {
        num = atoi(str);
    }
    if (int_part < 0) num = int_part * denum - num;
    else num = int_part * denum + num;
    if (denum == 0) {
        std::cout << "Dividing by zero\n";
        exit(0);
    }
    numerator = num;
    denumerator = denum;
    simplify();
}

Fraction::Fraction(const Fraction &fr) {
    numerator = fr.numerator;
    denumerator = fr.denumerator;
}

Fraction::Fraction(double dbl) {
    double i = std::floor(dbl);
    double frac = dbl - i;
    int acc = 1000000000;
    denumerator = acc;
    numerator = round(frac * acc);
    simplify();
    numerator += i * denumerator;
}


int Fraction::getNumerator() {
    return numerator;
}

void Fraction::setNumerator(int _numerator) {
    numerator = _numerator;
    simplify();
}

int Fraction::getDenumerator() const {
    return denumerator;
}

void Fraction::setDenumerator(int _denumerator) {
    denumerator = _denumerator;
    simplify();
}

Fraction Fraction::operator + (const Fraction &fr) {
    int new_denum = denumerator * fr.denumerator;
    int new_num = numerator * fr.denumerator + fr.numerator * denumerator;
    return Fraction(new_num, new_denum);
}

Fraction Fraction::operator + (double dbl) {
    Fraction fr = dbl;
    return *this + fr;
}

Fraction Fraction::operator + (int i) {
    return Fraction(numerator + i * denumerator, denumerator);
}

Fraction& Fraction::operator += (const Fraction &fr) {
    *this = *this + fr;
    return *this;
}

Fraction& Fraction::operator += (double dbl) {
    *this = *this + dbl;
    return *this;
}

Fraction& Fraction::operator += (int i) {
    *this = *this + i;
    return *this;
}

Fraction& Fraction::operator = (double dbl) {
    *this = Fraction(dbl);
    return *this;
}

Fraction& Fraction::operator = (const char *str) {
    *this = Fraction(str);
    return *this;
}

Fraction operator + (int i, const Fraction &fr) {
    return Fraction(i * fr.denumerator + fr.numerator, fr.denumerator);
}

Fraction operator + (double dbl, const Fraction &fr) {
    Fraction fr_left = dbl;
    return fr_left + fr;
}


std::istream& operator >> (std::istream &in, Fraction &fr) {
    char str[50];
    in.getline(str, 50);
    fr = str;
    return in;
}

std::ostream& operator << (std::ostream &out, const Fraction &fr) {
    int int_part = fr.numerator / fr.denumerator;
    int new_num = fr.numerator - int_part * fr.denumerator;
    if (int_part != 0) out << int_part << ' ';
    if (new_num != 0) out << new_num * (int_part >= 0 ? 1 : -1) << '/' << fr.denumerator;
    if (int_part == 0 && new_num == 0) out << 0;
    return out;
}
