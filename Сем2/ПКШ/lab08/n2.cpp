#include <iostream>


int main() {
    std::string str, ans;
    getline(std::cin, str);
    for (int i = 0; i < str.size(); i++) {
        if (isalnum(str[i])) {
            ans += str[i];
        }
    }
    std::cout << ans;
    return 0;
}
