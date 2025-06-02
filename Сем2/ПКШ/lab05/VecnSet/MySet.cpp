#include "MySet.h"

int MySet::q_find(const char *str) {
    int l = 0, r = size - 1;
    int mid = (l + r) / 2;
    while (strcmp(pdata[mid], str) && (l < r)){
        if (strcmp(pdata[mid], str) > 0){
            r = mid;
        }
        else{
            l = mid + 1;
        }
        mid = (l + r) / 2;
    }
    if (!strcmp(pdata[mid], str)) {
        return mid;
    }
    return -1;
}

void MySet::add_element(const char *str) {
    if (q_find(str) != -1) return;
    MyVector::add_element(str);
    sort();
}

void MySet::delete_element(const char *str) {
    int ind = q_find(str);
    MyVector::delete_element(ind);
}


bool MySet::operator == (MySet &s) {
    for (int i = 0; i < size; i++) {
        if (strcmp(pdata[i], s.pdata[i])) return false;
    }
    return true;
}

//MySet& MySet::operator = (const MySet &s) {
//    for (int i = 0; i < s.size; i++) {
//        add_element(s[i]);
//    }
//    return *this;
//}

MySet& MySet::operator += (MySet &s) {
    *this = *this + s;
    return *this;
}

MySet& MySet::operator -= (MySet &s) {
    *this = *this - s;
    return *this;
}

MySet& MySet::operator *= (MySet &s) {
    *this = *this * s;
    return *this;
}


std::istream& operator >> (std::istream &in, MySet &s) {
    int lines_cnt;
    in >> lines_cnt;
    in.ignore(1);
    char str[100];
    for (int i = 0; i < lines_cnt; i++) {
        in.getline(str, 100);
        s.add_element(str);
    }
    return in;
}


MySet operator + (MySet &s1, MySet &s2) {
    MySet tmp = s1;
    for (int i = 0; i < s2.size; i++) {
        tmp.add_element(s2[i]);
    }
    return tmp;
}

MySet operator - (MySet &s1, MySet &s2) {
    MySet tmp = s1;
    for (int i = 0; i < s2.size; i++) {
        tmp.delete_element(s2[i]);
    }
    return tmp;
}

MySet operator * (MySet &s1, MySet &s2) {
    MySet tmp;
    for (int i = 0; i < s1.size; i++) {
        if (s2.is_element(s1[i])) {
            tmp.add_element(s1[i]);
        }
    }
    return tmp;
}
