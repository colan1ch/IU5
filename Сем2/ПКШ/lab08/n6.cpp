#include <iostream>
#include <list>


void print_list(std::list<std::string> list) {
    int i = 0;
    for (auto val = list.begin(); val != list.end(); val++, i++) {
        std::cout << i << ": " << *val << '\n';
    }
}

int main() {
    std::list<std::string> list;
    std::string str;
    getline(std::cin, str);
    while (!str.empty()) {
        list.push_back(str);
        getline(std::cin, str);
    }
    print_list(list);
    std::cout << '\n';
    if (list.size() <= 3) {
        std::cout << "Too small list to delete 4th element\n";
        return 0;
    }
    auto iter = list.begin();
    ++iter;++iter;++iter;
    list.erase(iter);
    print_list(list);
    return 0;
}
