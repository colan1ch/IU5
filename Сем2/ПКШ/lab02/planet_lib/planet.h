#pragma once
#include <iostream>
#include <fstream>

class Planet {
    char *name;
    int diameter;
    bool life;
    int satellites_count;
    static unsigned total;
    unsigned id = 0;
public:
    Planet();

    Planet(const Planet&);

    ~Planet();

    Planet(const char *, int, bool, int);

    char *getName();

    int getDiameter();

    bool getLife();

    int getSatellitesCount();

    void setName(const char *);

    void setDiameter(int);

    void setLife(bool);

    void setSatellitesCount(int);
    
    void edit();

    bool operator < (const Planet&);

    bool operator == (const Planet&);

    Planet& operator = (const Planet&);


    static int read_db(const char *, Planet *, int);

    static void print_db(Planet *, int);

    static int write_db(const char *, Planet *, int);

    static int find(Planet *, int, const char *);

    static void sort_db(Planet *, int);

    static void add_planet(Planet *, int&, const Planet&);

    static void delete_planet(Planet *, int&, const char *);
};

std::ostream& operator << (std::ostream&, Planet&);

std::istream& operator >> (std::istream&, Planet&);
