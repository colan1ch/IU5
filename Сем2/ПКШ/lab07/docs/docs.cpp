#include "docs.h"

Document::Document() {
    number = 0;
    name = new char[1];
    name[0] = '\0';
}

Document::Document(int _number, const char *_name) {
    number = _number;
    name = new char[strlen(_name) + 1];
    strcpy(name, _name);
}

Document::Document(const Document &doc) {
    number = doc.number;
    name = new char[strlen(doc.name) + 1];
    strcpy(name, doc.name);
}

Document& Document::operator = (const Document &doc) {
    delete[] name;
    number = doc.number;
    name = new char[strlen(doc.name) + 1];
    strcpy(name, doc.name);
    return *this;
}


Receipt::Receipt() : Document(), cost(0) {
    product_name = new char[1];
    product_name[0] = '\0';
    add();
}

Receipt::Receipt(int _number, const char *_name, int _cost, const char *_product_name) : Document(_number, _name) {
    cost = _cost;
    product_name = new char[strlen(_product_name) + 1];
    strcpy(product_name, _product_name);
    add();
}

Receipt::Receipt(const Receipt &rec) : Document(rec) {
    cost = rec.cost;
    product_name = new char[strlen(rec.product_name) + 1];
    strcpy(product_name, rec.product_name);
    add();
}

Receipt& Receipt::operator = (const Receipt &rec) {
    delete[] name;
    delete[] product_name;
    number = rec.number;
    name = new char[strlen(rec.name) + 1];
    strcpy(name, rec.name);
    cost = rec.cost;
    product_name = new char[strlen(rec.product_name) + 1];
    strcpy(product_name, rec.product_name);
    return *this;
}


void Receipt::add() {
    if (Document::size == Document::capacity) {
        Document::capacity *= 2;
        Document **tmp = new Document*[Document::capacity];
        for (int i = 0; i < Document::size; i++) {
            tmp[i] = Document::docs[i];
        }
        tmp[Document::size++] = this;
        Document::docs = tmp;
    }
    else {
        Document::docs[Document::size++] = this;
    }
}

void Receipt::show() {
    std::cout << "Document type: Receipt\nDocument number: " << number << "\nDocument name: " << name << '\n';
    std::cout << "Cost: " << cost << "\nProduct name: " << product_name << "\n\n";
}


Check& Check::operator = (const Check &ch) {
    delete[] name;
    delete[] product_name;
    number = ch.number;
    name = new char[strlen(ch.name) + 1];
    strcpy(name, ch.name);
    cost = ch.cost;
    product_name = new char[strlen(ch.product_name) + 1];
    strcpy(product_name, ch.product_name);
    nds_percent = ch.nds_percent;
    return *this;
}

void Check::show() {
    std::cout << "Document type: Check\nDocument number: " << number << "\nDocument name: " << name << '\n';
    std::cout << "Cost: " << cost << "\nProduct name: " << product_name << "\nValue added tax: " << nds_percent << "%\n\n";
}

Invoice& Invoice::operator = (const Invoice &inv) {
    delete[] name;
    delete[] product_name;
    number = inv.number;
    name = new char[strlen(inv.name) + 1];
    strcpy(name, inv.name);
    cost = inv.cost;
    product_name = new char[strlen(inv.product_name) + 1];
    strcpy(product_name, inv.product_name);
    delivery_cost = inv.delivery_cost;
    return *this;
}

void Invoice::show() {
    std::cout << "Document type: Invoice\nDocument number: " << number << "\nDocument name: " << name << '\n';
    std::cout << "Cost: " << cost << "\nProduct name: " << product_name << "\nDelivery cost: " << delivery_cost << "\n\n";
}
