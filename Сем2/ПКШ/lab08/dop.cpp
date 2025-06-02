#include <iostream>


int main() {
    std::string str, ans;
    std::cin >> str;
    char cur_char = '0';
    int cur_cnt = 0;
    for (int i = 0; i < str.size();) {
        while (str[i] == cur_char) {
            cur_cnt++;
            i++;
        }
        if (cur_char != '0')
            ans += cur_char + std::to_string(cur_cnt);
        cur_char = str[i];
        cur_cnt = 0;
    }
    std::cout << ans;
}