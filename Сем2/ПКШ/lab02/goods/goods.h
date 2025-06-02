#pragma once
#include <iostream>
#include <fstream>


class Goods {
    char *name;
    int price;
    int count_in_stock;
    char *category;
    static unsigned total;
    unsigned id = 0;
public:
    Goods();

    Goods(const Goods&);

    ~Goods();

    Goods(const char *, int, int, char *);

    char *getName();

    int getPrice();

    int getCountInStock();

    char *getCategory();

    void setName(const char *);

    void setPrice(int);

    void setCountInStock(int);

    void setCategory(const char *);

    void edit();

    bool operator < (const Goods&);

    bool operator == (const Goods&);

    Goods& operator = (const Goods&);


    static int read_db(const char *, Goods *, int);

    static void print_db(Goods *, int);

    static int write_db(const char *, Goods *, int);

    static int find(Goods *, int);

    static void sort_db(Goods *, int);

    static void add_goods(Goods *, int&);

    static void delete_goods(Goods *, int&);
};

std::ostream& operator << (std::ostream& out, Goods &planet);

std::istream& operator >> (std::istream& out, Goods &planet);
