#include "planet.h"

Planet::Planet() {
    name = new char[1];
    name[0] = '\0';
    diameter = -1;
    life = false;
    satellites_count = -1;
    total++;
    id = total;
    //std::cout << "Created id: " << id << '\n';
}

Planet::Planet(const Planet &pl) {
    name = new char[strlen(pl.name) + 1];
    strcpy(name, pl.name);
    diameter = pl.diameter;
    life = pl.life;
    satellites_count = pl.satellites_count;
    total++;
    id = total;
    //std::cout << "Created id: " << id << '\n';
}


Planet::~Planet() {
    delete[] name;
    total--;
    //std::cout << "Deleted id: " << id << '\n';
}


Planet::Planet(const char *_name, int _diameter, bool _life, int _satellites_count) {
    name = new char[strlen(_name) + 1];
    strcpy(name, _name);
    diameter = _diameter;
    life = _life;
    satellites_count = _satellites_count;
    total++;
    id = total;
    //std::cout << "Created id: " << id << '\n';
}

char *Planet::getName() {
    return name;
}

int Planet::getDiameter() {
    return diameter;
}

bool Planet::getLife() {
    return int(life);
}

int Planet::getSatellitesCount() {
    return satellites_count;
}

void Planet::setName(const char *_name) {
    name = new char[strlen(_name) + 1];
    strcpy(name, _name);
}

void Planet::setDiameter(int _diameter) {
    diameter = _diameter;
}

void Planet::setLife(bool _life) {
    life = _life;
}

void Planet::setSatellitesCount(int _satellites_count) {
    satellites_count = _satellites_count;
}


void Planet::edit() {
    std::cout << "Введите новые значения для данной планеты\n";
    std::cin >> *this;
}


std::ostream& operator << (std::ostream& out, Planet &planet) {
    out << planet.getName() << ' ' << planet.getDiameter() << ' ' << planet.getLife() << ' ' << planet.getSatellitesCount();
    return out;
}


std::istream& operator >> (std::istream& in, Planet &planet) {
    char *name = new char[15];
    int diameter;
    bool life;
    int satellites_count;
    in >> name >> diameter >> life >> satellites_count;
    planet.setName(name);
    planet.setDiameter(diameter);
    planet.setLife(life);
    planet.setSatellitesCount(satellites_count);
    return in;
}

bool Planet::operator < (const Planet &pl) {
    return diameter < pl.diameter;
}

bool Planet::operator == (const Planet &pl) {
    return ((!strcmp(name, pl.name)) && (diameter == pl.diameter) && (life == pl.life) && (satellites_count == pl.satellites_count));
}


Planet& Planet::operator = (const Planet &pl) {
    delete[] name;
    name = new char[strlen(pl.name) + 1];
    strcpy(name, pl.name);
    diameter = pl.diameter;
    life = pl.life;
    satellites_count = pl.satellites_count;
    return *this;
}

int Planet::read_db(const char *file_name, Planet *planets, int size) {
    std::ifstream fin(file_name);
    int i = 0;
    while (fin >> planets[i++]);
    return i - 1;
}


void Planet::print_db(Planet *planets, int planets_count) {
    for (int i = 0; i < planets_count; i++) {
        std::cout << planets[i] << '\n';
    }
}


int Planet::write_db(const char *file_name, Planet *planets, int planets_count) {
    std::ofstream fout(file_name);
    if (!fout) return -1;
    for (int i = 0; i < planets_count; i++) {
        fout << planets[i] << '\n';
    }
    return 0;
}


int Planet::find(Planet *planets, int planets_count, const char *search_name) {
    for (int i = 0; i < planets_count; i++) {
        if (!strcmp(search_name, planets[i].getName())) {
            return i;
        }
    }
    return -1;
}


void Planet::sort_db(Planet *planets, int planets_count) {
    for (int i = 0; i < planets_count; i++) {
        for (int j = i + 1; j < planets_count; j++) {
            if (!(planets[i] < planets[j])) {
                Planet tmp(planets[i]);
                planets[i] = planets[j];
                planets[j] = tmp;
            }
        }
    }
}


void Planet::add_planet(Planet *planets, int &planets_count, const Planet &new_planet) {
    planets[planets_count++] = new_planet;
}

void Planet::delete_planet(Planet *planets, int &planets_count, const char *name_to_delete) {
    int ind = Planet::find(planets, planets_count, name_to_delete);
    if (ind == -1) {
        std::cout << "Такой планеты нет\n";
        return;
    }
    planets_count--;
    for (int i = ind; i < planets_count; i++) {
        planets[i] = planets[i + 1];
    }
}
