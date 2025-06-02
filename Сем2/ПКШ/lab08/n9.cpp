#include <iostream>
#include <string>
#include <map>


void print_map(std::map<std::string, int> map) {
    for (auto i = map.begin(); i != map.end(); i++) {
        if (i->second != 0) {
            std::cout << i->first << " -> " << i->second << '\n';
        }
    }
}


int main() {
    std::map<std::string, int> pupils_cnt = {{"1а", 21}, {"1б", 20}, {"2", 25}, {"3а", 20}, {"3б", 23}, {"4а", 25},
                                             {"4б", 25}, {"5а", 26}, {"5б", 25}, {"6a", 19}, {"6б", 18}, {"7а", 26},
                                             {"7б", 25}, {"8", 25}, {"9", 17}, {"10", 15}, {"11", 14}};
    int ans = 0;
    for (auto i = pupils_cnt.begin(); i != pupils_cnt.end(); i++) {
        ans += i->second;
    }
    print_map(pupils_cnt);
    std::cout << '\n' << ans;
}
