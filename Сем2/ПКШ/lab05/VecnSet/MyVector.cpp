#include "MyVector.h"

MyVector::MyVector() {
    pdata = new char *[1];
    pdata[0] = new char[1];
    pdata[0][0] = '\0';
    size = 0;
    max_size = 1;
}


MyVector::MyVector(const char *str) {
    pdata = new char *[1];
    pdata[0] = new char[strlen(str) + 1];
    strcpy(pdata[0], str);
    size = 1;
    max_size = 2;
}

MyVector::MyVector(const MyVector &vec) {
    pdata = new char *[vec.max_size];
    size = vec.size;
    max_size = vec.max_size;
    for (int i = 0; i < vec.size; i++) {
        pdata[i] = new char[strlen(vec.pdata[i]) + 1];
        strcpy(pdata[i], vec.pdata[i]);
    }
}

MyVector::~MyVector() {
    for (int i = 0; i < size; i++) {
        delete[] pdata[i];
    }
    delete[] pdata;
}

void MyVector::sort() {
    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size; j++) {
            if (strcmp(pdata[i], pdata[j]) > 0) {
                std::swap(pdata[i], pdata[j]);
            }
        }
    }
}

void MyVector::add_element(const char *str) {
    resize();
    pdata[size] = new char[strlen(str) + 1];
    strcpy(pdata[size], str);
    size++;
}

bool MyVector::delete_element(int ind) {
    if (ind < 0 || ind >= size) return false;
    for (int i = ind + 1; i < size; i++) {
        strcpy(pdata[i - 1], pdata[i]);
    }
    size--;
    resize();
    return true;
}

int MyVector::find(const char *str) {
    for (int i = 0; i < size; i++) {
        if (!strcmp(pdata[i], str)) {
            return i;
        }
    }
    return -1;
}

void MyVector::resize() {
    if (size >= max_size) {
        max_size *= 2;
    }
    else if (size < max_size / 4) {
        max_size /= 2;
    }
    else {
        return;
    }
    char **tmp = new char *[max_size];
    for (int i = 0; i < size; i++) {
        tmp[i] = new char[strlen(pdata[i]) + 1];
        strcpy(tmp[i], pdata[i]);
        delete[] pdata[i];
    }
    delete[] pdata;
    pdata = tmp;
}

int MyVector::len() {
    return size;
}

MyVector& MyVector::operator = (const MyVector& vec) {
    for (int i = 0; i < size; i++) {
        delete[] pdata[i];
    }
    delete[] pdata;
    pdata = new char *[vec.max_size];
    size = vec.size;
    max_size = vec.max_size;
    for (int i = 0; i < vec.size; i++) {
        pdata[i] = new char[strlen(vec.pdata[i]) + 1];
        strcpy(pdata[i], vec.pdata[i]);
    }
    return *this;
}

char* MyVector::operator[] (int i) {
    if (i < 0 || i >= size) {
        std::cout << "Index Error\n";
        return "";
    }
    return pdata[i];
}

std::istream& operator >> (std::istream &in, MyVector &vec) {
    int lines_cnt;
    in >> lines_cnt;
    in.ignore(1);
    char str[100];
    for (int i = 0; i < lines_cnt; i++) {
        in.getline(str, 100);
        vec.add_element(str);
    }
    return in;
}

std::ostream& operator << (std::ostream &out, MyVector &vec) {
    for (int i = 0; i < vec.size; i++) {
        out << vec[i] << ' ';
    }
    out << '\n';
    return out;
}
