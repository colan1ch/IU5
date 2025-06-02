#include "planet_lib/planet.h"
#include "goods/goods.h"


const int SIZE = 12;

unsigned Planet::total = 0;

unsigned Goods::total = 0;


int menu() {
    int k;
    std::cout << "Введите номер команды\n";
    std::cout << "1 - Взять значения из базы данных\n";
    std::cout << "2 - Записать значения в базу данных\n";
    std::cout << "3 - Найти товар по названию\n";
    std::cout << "4 - Вывести информацию о товарах на экран\n";
    std::cout << "5 - Отсортировать товары по возрастанию цены\n";
    std::cout << "6 - Добавить товар\n";
    std::cout << "7 - Удалить товар\n";
    std::cout << "8 - Изменить значения товара\n";
    std::cout << "9 - Выход\n";
    std::cin >> k;
    return k;
}

int demo() {
    Planet planets[SIZE];
    char file_name[15] = "sunsys.txt";
    std::cout << "\treading db\n";
    int planets_count = Planet::read_db(file_name, planets, SIZE);
    std::cout << "\tprinting db\n";
    Planet::print_db(planets, planets_count);
    std::cout << "\tsorting db\n";
    Planet::sort_db(planets, planets_count);
    std::cout << "\tprinting db\n";
    Planet::print_db(planets, planets_count);
    std::cout << "\tdeleting Venus\n";
    Planet::delete_planet(planets, planets_count, "Venus");
    std::cout << "\tprinting db\n";
    Planet::print_db(planets, planets_count);
    std::cout << "\tadding planet 'zxc'\n";
    Planet zxc;
    zxc.setName("zxc");
    zxc.setDiameter(123);
    zxc.setLife(true);
    zxc.setSatellitesCount(11);
    Planet::add_planet(planets, planets_count, zxc);
    std::cout << "\tprinting db\n";
    Planet::print_db(planets, planets_count);
    std::cout << "\tsearching Earth index and printing: ";
    std::cout << Planet::find(planets, planets_count, "Earth");
    std::cout << "\n\twriting db to file\n";
    Planet::print_db(planets, planets_count);
    return 0;
}


int interactive() {
    char file_name[15] = "goods.txt";
    Goods goods[SIZE];
    int goods_count;
    int ind;
    while (true) {
        switch (menu()) {
            case 1:
                goods_count = Goods::read_db(file_name, goods, SIZE);
                break;
            case 2:
                if (Goods::write_db(file_name, goods, goods_count)) {
                    std::cout << "ERROR\n";
                }
                break;
            case 3:
                if ((ind = Goods::find(goods, goods_count)) >= 0)
                    std::cout << "Искомая планета имеет индекс " << ind << '\n';
                else
                    std::cout << "Такой планеты нет" << std::endl;
                break;
            case 4:
                Goods::print_db(goods, goods_count);
                break;
            case 5:
                Goods::sort_db(goods, goods_count);
                break;
            case 6:
                Goods::add_goods(goods, goods_count);
                break;
            case 7:
                Goods::delete_goods(goods, goods_count);
                break;
            case 8:
                if ((ind = Goods::find(goods, goods_count)) >= 0)
                    goods[ind].edit();
                else
                    std::cout << "Такой планеты нет" << std::endl;
                break;
            case 9:
                return 0;
            default:
                std::cout << " Неправильный ввод" << std::endl;
                break;
        }
    }
}


int main(int argc, char **argv) {
    if (argc != 2) return 0;
    if (!strcmp(argv[1], "demo")) {
        demo();
    }
    else if (!strcmp(argv[1], "int")) {
        interactive();
    }
    return 0;
}
