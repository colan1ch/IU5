#include <fstream>


int main() {
    std::ifstream fin("n3.txt");
    std::ofstream fout("n3_ans.txt");
    std::string str;
    while (getline(fin, str)) {
        if (str.find('"') != -1) fout << str << '\n';
    }
    return 0;
}
