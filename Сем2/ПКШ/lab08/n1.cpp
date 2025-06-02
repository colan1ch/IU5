#include <iostream>


int main() {
    std::string str;
    getline(std::cin, str);
    int ind_point = str.find('.');
    int ind1 = 0, ind2 = ind_point + 1;
    while (str[ind1] == ' ') {
        ind1++;
    }
    while (str[ind2] == ' ') {
        ind2++;
    }
    std::swap(str[ind1], str[ind2]);
    std::cout << str;
    return 0;
}
