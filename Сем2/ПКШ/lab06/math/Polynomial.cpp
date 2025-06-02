#include "Polynomial.h"


Polynomial::Polynomial(int multiplier) {
    poly = new Term[1];
    poly[0].multiplier = multiplier;
    poly[0].power = 0;
    degree = 0;
    size = 1;
}

Polynomial::Polynomial(const Term &t) {
    poly = new Term[1];
    poly[0] = t;
    degree = t.power;
    size = 1;
}


Polynomial::Polynomial(const Polynomial &p) {
    poly = new Term[p.size];
    for (int i = 0; i < p.size; i++) {
        poly[i] = p.poly[i];
    }
    degree = p.degree;
    size = p.size;
}

Polynomial& Polynomial::operator = (const Polynomial &p) {
    if (size > 0) {
        delete[] poly;
    }
    poly = new Term[p.size];
    for (int i = 0; i < p.size; i++) {
        poly[i] = p.poly[i];
    }
    degree = p.degree;
    size = p.size;
    return *this;
}


Polynomial& Polynomial::operator += (const Polynomial &p) {
    *this = *this + p;
    return *this;
}

Polynomial& Polynomial::operator *= (const Polynomial &p) {
    *this = *this * p;
    return *this;
}

Polynomial operator + (const Polynomial &p1, const Polynomial &p2) {
    Polynomial tmp;
    int k = tmp.order_increase ? -1 : 1;
    tmp.poly = new Term[p1.size + p2.size];
    int i1 = 0, i2 = 0, i = 0;
    while (i1 < p1.size && i2 < p2.size) {
        if (p1.poly[i1].power * k < p2.poly[i2].power * k) {
            tmp.poly[i++] = p1.poly[i1++];
        }
        else if (p1.poly[i1].power == p2.poly[i2].power) {
            tmp.poly[i++] = p1.poly[i1++] + p2.poly[i2++];
        }
        else {
            tmp.poly[i++] = p2.poly[i2++];
        }
    }
    while (i1 < p1.size) {
        tmp.poly[i++] = p1.poly[i1++];
    }
    while (i2 < p2.size) {
        tmp.poly[i++] = p2.poly[i2++];
    }
    tmp.degree = (p1.degree > p2.degree) ? p1.degree : p2.degree;
    tmp.size = i;
    return tmp;
}

Polynomial operator * (const Polynomial &p1, const Polynomial &p2) {
    Polynomial tmp;
    for (int i = 0; i < p1.size; i++) {
        for (int j = 0; j < p2.size; j++) {
            tmp += Term(p1.poly[i].multiplier * p2.poly[j].multiplier, p1.poly[i].power + p2.poly[j].power);
        }
    }
    return tmp;
}

std::istream& operator >> (std::istream &in, Polynomial &p) {
    if (p.size > 0) {
        delete[] p.poly;
        p.size = 0;
    }
    char str[1000], cur_term[100];
    bool is_pos = true, is_power = false, is_0_power = true;
    int ind = 0, cur_mul = 1, cur_power = 0;
    in.getline(str, 1000);
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == '+' || str[i] == '-') {
            if (is_0_power) {
                cur_power = 0;
                cur_term[ind] = '\0';
                cur_mul = atoi(cur_term);
            }
            else if (is_power) {
                cur_term[ind] = '\0';
                cur_power = atoi(cur_term);
            }
            else {
                cur_power = 1;
            }
            if (!is_pos) cur_mul *= -1;
            Term t(cur_mul, cur_power);
            p += t;
            ind = 0;
            is_pos = (str[i] == '+');
            is_0_power = true;
            is_power = false;
            cur_mul = 1;
            cur_power = 0;
        }
        else {
            if (str[i] == 'x') {
                cur_term[ind] = '\0';
                if (ind == 0) cur_mul = 1;
                else cur_mul = atoi(cur_term);
                is_0_power = false;
            }
            else if (str[i] == '^') {
                ind = 0;
                is_power = true;
            }
            else if (str[i] >= '0' && str[i] <= '9') cur_term[ind++] = str[i];
        }
    }
    if (!is_power) {
        cur_term[ind] = '\0';
        if (ind == 0) cur_mul = 1;
        else cur_mul = atoi(cur_term);
        if (!is_0_power) cur_power = 1;
    }
    else {
        if (is_0_power) cur_power = 0;
        else {
            cur_term[ind] = '\0';
            if (ind == 0) cur_power = 1;
            else cur_power = atoi(cur_term);
        }
    }
    if (!is_pos) cur_mul *= -1;
    Term t(cur_mul, cur_power);
    p += t;
    return in;
}


std::ostream& operator << (std::ostream &out, const Polynomial &p) {
    int ind = 0;
    while (ind < p.size - 1 && p.poly[ind].multiplier == 0) ind++;
    out << p.poly[ind];
    for (int i = ind + 1; i < p.size; i++) {
        if (p.poly[i].multiplier < 0) out << p.poly[i];
        else if (p.poly[i].multiplier > 0) out << '+' << p.poly[i];
    }
    return out;
}
