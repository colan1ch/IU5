#include "MyVector.h"


class MySet: public MyVector{
private:
    int q_find(const char *str);
public:
    MySet() : MyVector() {}

    MySet(const char *str) : MyVector(str) {}

    bool is_element(const char *str) { return q_find(str) != -1; }

    void add_element(const char *str);

    void delete_element(const char *str);


    bool operator == (MySet &s);

    //MySet& operator = (const MySet &s);

    MySet& operator += (MySet &s);

    MySet& operator -= (MySet &s);

    MySet& operator *= (MySet &s);


    friend std::istream& operator >> (std::istream &in, MySet &s);

    friend MySet operator + (MySet &s1, MySet &s2);

    friend MySet operator - (MySet &s1, MySet &s2);

    friend MySet operator * (MySet &s1, MySet &s2);
};
