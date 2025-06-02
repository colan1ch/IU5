#pragma once
#include <iostream>


template<class T>
int compare(T val1, T val2) {
    return val1 > val2 ? 1 : (val1 < val2 ? -1 : 0);
}

template<>
int compare<char *>(char *val1, char *val2) {
    if (!val1 || !val2) return -1;
    return strcmp(val1, val2);
}


template <class T>
class MyVector {
protected:
    int max_size;
    int size;
    T *pdata;

    void resize() {
        if (size >= max_size - 1) {
            max_size *= 2;
        }
        else if (size < max_size / 4) {
            max_size /= 2;
        }
        else {
            return;
        }
        T *tmp = new T[max_size];
        for (int i = 0; i < size; i++) {
            tmp[i] = pdata[i];
        }
        //delete[] pdata;
        pdata = tmp;
    }
public:
    MyVector() {
        pdata = new T[1];
        size = 0;
        max_size = 1;
    }

    MyVector(const T &val) {
        pdata = new T[1];
        pdata[0] = val;
        size = 1;
        max_size = 1;
    }

    MyVector(const MyVector &vec) {
        pdata = new T[vec.max_size];
        size = vec.size;
        max_size = vec.max_size;
        for (int i = 0; i < vec.size; i++) {
            pdata[i] = vec.pdata[i];
        }
    }

    ~MyVector() {
        delete[] pdata;
    }

    void sort() {
        for (int i = 0; i < size; i++) {
            for (int j = i + 1; j < size; j++) {
                if (compare(pdata[i], pdata[j]) > 0) {
                    std::swap(pdata[i], pdata[j]);
                }
            }
        }
    }

    void add_element(const T &val) {
        resize();
        pdata[size++] = val;
    }

    bool delete_element(int ind) {
        if (ind < 0 || ind >= size) return false;
        for (int i = ind + 1; i < size; i++) {
            pdata[i - 1] = pdata[i];
        }
        size--;
        resize();
        return true;
    }

    int find(const T &val) {
        for (int i = 0; i < size; i++) {
            if (compare(pdata[i], val) == 0) {
                return i;
            }
        }
        return -1;
    }

    int len() {
        return size;
    }

    MyVector& operator = (const MyVector &vec) {
        delete[] pdata;
        pdata = new T[vec.max_size];
        size = 0;
        max_size = vec.max_size;
        for (int i = 0; i < vec.size; i++) {
            add_element(vec.pdata[i]);
        }
        return *this;
    }

    T& operator[] (int i) {
        if (i < 0 || i >= size) {
            std::cout << "Index Error\n";
            exit(1);
        }
        return pdata[i];
    }


    friend std::ostream& operator << (std::ostream &out, MyVector &vec) {
        for (int i = 0; i < vec.size; i++) {
            out << vec[i] << ' ';
        }
        out << '\n';
        return out;
    }
};


template<>
MyVector<char *>::MyVector() {
    pdata = new char *[1];
    pdata[0] = new char[1];
    pdata[0][0] = '\0';
    size = 0;
    max_size = 1;
}

template<>
MyVector<char *>::MyVector(char *const &str) {
    pdata = new char *[1];
    pdata[0] = new char[strlen(str) + 1];
    strcpy(pdata[0], str);
    size = 1;
    max_size = 1;
}

template<>
MyVector<char *>::MyVector(const MyVector &vec) {
    pdata = new char *[vec.max_size];
    size = vec.size;
    max_size = vec.max_size;
    for (int i = 0; i < vec.size; i++) {
        pdata[i] = new char[strlen(vec.pdata[i]) + 1];
        strcpy(pdata[i], vec.pdata[i]);
    }
}

template<>
MyVector<char *>::~MyVector() {
    for (int i = 0; i < size; i++) {
        delete[] pdata[i];
    }
    delete[] pdata;
}

template<>
void MyVector<char *>::add_element(char *const &str) {
    resize();
    pdata[size] = new char[strlen(str) + 1];
    strcpy(pdata[size], str);
    size++;
}

template<>
bool MyVector<char *>::delete_element(int ind) {
    if (ind < 0 || ind >= size) return false;
    if (pdata[ind]) delete[] pdata[ind];
    for (int i = ind + 1; i < size; i++) {
        pdata[i - 1] = pdata[i];
    }
    size--;
    resize();
    return true;
}

template<>
MyVector<char *>& MyVector<char *>::operator = (const MyVector& vec) {
    for (int i = 0; i < size; i++) {
        delete[] pdata[i];
    }
    delete[] pdata;
    pdata = new char *[vec.max_size];
    size = 0;
    max_size = vec.max_size;
    for (int i = 0; i < vec.size; i++) {
        add_element(vec.pdata[i]);
    }
    return *this;
}
