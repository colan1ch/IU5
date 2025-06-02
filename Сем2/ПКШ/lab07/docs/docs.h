#pragma once
#include <iostream>


class Document{
protected:
    int number;
    char *name;
public:
    static Document **docs;
    static int size;
    static int capacity;

    Document();

    Document(int _number, const char *_name);

    Document(const Document &doc);

    virtual ~Document() { delete[] name; }

    virtual void add() = 0;

    virtual void show() = 0;

    Document& operator = (const Document &doc);

    static void print() {
        for (int ind = 0; ind < size; ind++) {
            docs[ind]->show();
        }
    }
};


class Receipt: public Document{
protected:
    int cost;
    char *product_name;
public:
    Receipt();

    Receipt(int _number, const char *_name, int _cost, const char *_product_name);

    Receipt(const Receipt &rec);

    Receipt& operator = (const Receipt &rec);

    void add();

    void show();

    ~Receipt() { delete[] product_name; }
};


class Check: public Receipt{
    int nds_percent;
public:
    Check() : Receipt(), nds_percent(0) {}

    Check(int _number, const char *_name, int _cost, const char *_product_name, int _nds) :
    Receipt(_number, _name, _cost, _product_name), nds_percent(_nds) {}

    Check(const Check &ch) : Receipt(ch), nds_percent(ch.nds_percent) {}

    Check& operator = (const Check &ch);

    void show();
};


class Invoice: public Receipt{
    int delivery_cost;
public:
    Invoice() : Receipt(), delivery_cost(0) {}

    Invoice(int _number, const char *_name, int _cost, const char *_product_name, int _delivery_cost) :
    Receipt(_number, _name, _cost, _product_name), delivery_cost(_delivery_cost) {}

    Invoice(const Invoice &inv) : Receipt(inv), delivery_cost(inv.delivery_cost) {}

    Invoice& operator = (const Invoice &inv);

    void show();
};
