#include "goods.h"

Goods::Goods() {
    name = new char[1];
    name[0] = '\0';
    price = 0;
    count_in_stock = 0;
    category = new char[1];
    category[0] = '\0';
    total++;
    id = total;
    //std::cout << "Created id: " << id << '\n';
}

Goods::Goods(const Goods &gd) {
    name = new char[strlen(gd.name) + 1];
    strcpy(name, gd.name);
    price = gd.price;
    count_in_stock = gd.count_in_stock;
    category = new char[strlen(gd.category) + 1];
    strcpy(category, gd.category);
    total++;
    id = total;
    //std::cout << "Created id: " << id << '\n';
}


Goods::~Goods() {
    delete[] name;
    delete[] category;
    total--;
    //std::cout << "Deleted id: " << id << '\n';
}


Goods::Goods(const char *_name, int _price, int _count_in_stock, char *_category) {
    name = new char[strlen(_name) + 1];
    strcpy(name, _name);
    price = _price;
    count_in_stock = _count_in_stock;
    category = new char[strlen(_category) + 1];
    strcpy(category, _category);
    total++;
    id = total;
    //std::cout << "Created id: " << id << '\n';
}

char *Goods::getName() {
    return name;
}

int Goods::getPrice() {
    return price;
}

int Goods::getCountInStock() {
    return count_in_stock;
}

char *Goods::getCategory() {
    return category;
}

void Goods::setName(const char *_name) {
    delete[] name;
    name = new char[strlen(_name) + 1];
    strcpy(name, _name);
}

void Goods::setPrice(int _price) {
    price = _price;
}

void Goods::setCountInStock(int _count_in_stock) {
    count_in_stock = _count_in_stock;
}

void Goods::setCategory(const char *_category) {
    delete[] category;
    category = new char[strlen(_category) + 1];
    strcpy(category, _category);
}

void Goods::edit() {
    std::cout << "Введите новые значения для данного товара\n";
    std::cin >> (*this);
}


std::ostream& operator << (std::ostream& out, Goods &gd) {
    out << gd.getName() << ' ' << gd.getPrice() << ' ' << gd.getCountInStock() << ' ' << gd.getCategory();
    return out;
}


std::istream& operator >> (std::istream& in, Goods &gd) {
    char name[20];
    int price, count_in_stock;
    char *category = new char[30];
    in >> name >> price >> count_in_stock >> category;
    gd.setName(name);
    gd.setPrice(price);
    gd.setCountInStock(count_in_stock);
    gd.setCategory(category);
    return in;
}

bool Goods::operator < (const Goods &gd) {
    return price < gd.price;
}

bool Goods::operator == (const Goods &gd) {
    return ((!strcmp(name, gd.name)) && (price == gd.price) && (count_in_stock == gd.count_in_stock) && (!strcmp(category, gd.category)));
}

Goods& Goods::operator = (const Goods &gd) {
    delete[] name;
    name = new char[strlen(gd.name) + 1];
    strcpy(name, gd.name);
    price = gd.price;
    count_in_stock = gd.count_in_stock;
    delete[] category;
    category = new char[strlen(gd.category) + 1];
    strcpy(category, gd.category);
    return *this;
}

int Goods::read_db(const char *file_name, Goods *goods, int size) {
    std::ifstream fin(file_name);
    int i = 0;
    while (fin >> goods[i++]);
    return i - 1;
}


void Goods::print_db(Goods *goods, int goods_count) {
    for (int i = 0; i < goods_count; i++) {
        std::cout << goods[i] << '\n';
    }
}


int Goods::write_db(const char *file_name, Goods *goods, int goods_count) {
    std::ofstream fout(file_name);
    if (!fout) return -1;
    for (int i = 0; i < goods_count; i++) {
        fout << goods[i] << '\n';
    }
    return 0;
}


int Goods::find(Goods *goods, int goods_count) {
    char search_name[15];
    std::cout << "Введите название товара, который хотите найти/удалить: \n";
    std::cin >> search_name;
    for (int i = 0; i < goods_count; i++) {
        if (!strcmp(search_name, goods[i].getName())) {
            return i;
        }
    }
    return -1;
}


void Goods::sort_db(Goods *goods, int goods_count) {
    for (int i = 0; i < goods_count; i++) {
        for (int j = i + 1; j < goods_count; j++) {
            if (!(goods[i] < goods[j])) {
                Goods tmp(goods[i]);
                goods[i] = goods[j];
                goods[j] = tmp;
            }
        }
    }
}


void Goods::add_goods(Goods *goods, int &goods_count) {
    std::cout << "Введите данные нового товара\n";
    Goods gd;
    std::cin >> gd;
    goods[goods_count++] = gd;
}

void Goods::delete_goods(Goods *goods, int &goods_count) {
    int ind = Goods::find(goods, goods_count);
    if (ind == -1) {
        std::cout << "Такого товара нет\n";
        return;
    }
    goods_count--;
    for (int i = ind; i < goods_count; i++) {
        goods[i] = goods[i + 1];
    }
}
