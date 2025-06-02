#include "MyVector.h"


template<class T>
class MySet : public MyVector<T> {
    int q_find(const T &val) {
        int l = 0, r = this->size - 1;
        int mid = (l + r) / 2;
        while ((compare(this->pdata[mid], val) != 0) && (l < r)){
            if (compare(this->pdata[mid], val) > 0){
                r = mid;
            }
            else{
                l = mid + 1;
            }
            mid = (l + r) / 2;
        }
        if (compare(this->pdata[mid], val) == 0) {
            return mid;
        }
        return -1;
    }
public:
    MySet() : MyVector<T>() {}

    MySet(const T &val) : MyVector<T>(val) {}

    MySet(const MySet &s) : MyVector<T>(s) {}

    bool is_element(const T &val) { return q_find(val) != -1; }

    void add_element(const T &val) {
        if (is_element(val)) return;
        this->size++;
        this->resize();
        int ind = 0;
        while ((this->pdata[ind] < val) && (ind < this->size - 1)) {
            ind++;
        }
        for (int i = this->size - 1; i > ind; i--) {
            this->pdata[i] = this->pdata[i - 1];
        }
        this->pdata[ind] = val;
    }

    bool delete_element(const T &val) {
        int ind = q_find(val);
        if (ind == -1) return false;
        return MyVector<T>::delete_element(ind);
    }

    bool operator == (const MySet &s) {
        if (this->size != s.size) return false;
        for (int i = 0; i < this->size; i++) {
            if (compare(this->pdata[i], s.pdata[i]) != 0) return false;
        }
        return true;
    }

    MySet& operator += (MySet &s) {
        *this = *this + s;
        return *this;
    }

    MySet& operator -= (MySet &s) {
        *this = *this - s;
        return *this;
    }

    MySet& operator *= (MySet &s) {
        *this = *this * s;
        return *this;
    }

    friend MySet operator + (MySet &s1, MySet &s2) {
        MySet tmp = s1;
        for (int i = 0; i < s2.size; i++) {
            tmp.add_element(s2.pdata[i]);
        }
        return tmp;
    }

    friend MySet operator - (MySet &s1, MySet &s2) {
        MySet tmp = s1;
        for (int i = 0; i < s2.size; i++) {
            tmp.delete_element(s2.pdata[i]);
        }
        return tmp;
    }

    friend MySet operator * (MySet &s1, MySet &s2) {
        MySet tmp;
        for (int i = 0; i < s1.size; i++) {
            if (s2.is_element(s1.pdata[i])) {
                tmp.add_element(s1.pdata[i]);
            }
        }
        return tmp;
    }

    MySet& operator = (const MySet &vec) {
        MyVector<T>::operator=(vec);
        return *this;
    }
};

template<>
void MySet<char *>::add_element(char *const &el) {
    if (is_element(el)) return;
    resize();
    size++;
    int ind = 0;
    while ((ind < size - 1) && (strcmp(pdata[ind], el) < 0)) {
        ind++;
    }
    for (int i = size - 1; i > ind; i--) {
        pdata[i] = pdata[i - 1];
    }
//    delete[] pdata[ind];
    pdata[ind] = new char[strlen(el) + 1];
    strcpy(pdata[ind], el);
}
