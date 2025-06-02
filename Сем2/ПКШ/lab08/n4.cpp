#include <iostream>

void print_vector(std::vector<std::string> v) {
    for (int i = 0; i < v.size(); i++) std::cout << i << ": " << v[i] << '\n';
}

int main() {
    std::vector<std::string> v;
    std::string str;
    getline(std::cin, str);
    while (!str.empty()) {
        v.push_back(str);
        getline(std::cin, str);
    }
    print_vector(v);
    std::cout << '\n';
    if (v.size() <= 3) {
        std::cout << "Too small vector to delete 4th element\n";
        return 0;
    }
    v.erase(v.begin() + 3);
    print_vector(v);
    return 0;
}
