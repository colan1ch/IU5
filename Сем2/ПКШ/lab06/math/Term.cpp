#include "Term.h"


Term Term::operator + (const Term &t) const{
    if (power != t.power) return Term();
    return Term(multiplier + t.multiplier, power);
}

std::istream& operator >> (std::istream &in, Term &t) {
    char str[1000], cur_term[100];
    bool is_pos = true, is_power = false, is_0_power = true;
    int ind = 0, cur_mul = 1, cur_power = 0;
    in.getline(str, 1000);
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == ' ' && (str[i + 1] == '+' || str[i + 1] == '-')) {
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
            t.multiplier = cur_mul;
            t.power = cur_power;
            return in;
        }
        else {
            if (str[i] == '-') is_pos = false;
            else if (str[i] == '+') is_pos = true;
            else if (str[i] == 'x') {
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
    t.multiplier = cur_mul;
    t.power = cur_power;
    return in;
}


std::ostream& operator << (std::ostream &out, const Term &t) {
    if (t.multiplier == 0) return out;
    if (t.power == 0) {
        out << t.multiplier;
    }
    else {
        if (t.multiplier == 1) out << 'x';
        else if (t.multiplier == -1) out << "-x";
        else out << t.multiplier << 'x';
        if (t.power > 1) out << '^' << t.power;
    }
    return out;
}
