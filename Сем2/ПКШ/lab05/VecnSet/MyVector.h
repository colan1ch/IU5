#pragma once
#include <iostream>


class MyVector{
protected:
    int max_size;
    int size;
    char **pdata;

    void resize();
public:
    MyVector();

    MyVector(const char *str);

    MyVector(const MyVector &vec);

    ~MyVector();

    void sort();

    void add_element(const char *str);

    bool delete_element(int ind);

    int find(const char *str);

    int len();

    MyVector& operator = (const MyVector &vec);

    char* operator[] (int i);


    friend std::istream& operator >> (std::istream &in, MyVector &vec);

    friend std::ostream& operator << (std::ostream &out, MyVector &vec);
};
