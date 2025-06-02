#include <iostream>
#include <map>


void print_map(std::map<std::string, int> map) {
    for (auto i = map.begin(); i != map.end(); i++) {
        if (i->second != 0) {
            std::cout << i->first << " - " << i->second << '\n';
        }
    }
}

int main() {
    std::map<std::string, int> inventory;
    inventory["Отверка"] = 0;
    inventory["Молоток"] = 10;
    inventory["Гайки"] = 100;
    inventory["Профиль"] = 0;
    inventory["Уголок"] = 9;
    inventory["Доска"] = 17;
    print_map(inventory);
    return 0;
}
