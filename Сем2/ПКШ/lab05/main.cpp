#include "VecnSet/MyVector.h"
#include "VecnSet/MySet.h"

int main() {
    MySet s1, s2;
    std::cin >> s1 >> s2;
    MySet plus = s1 + s2, minus;
    minus = s1 - s2;
    MySet pro = s1;
    pro *= s2;
    std::cout << plus << '\n' << minus << '\n' << pro;
    return 0;
}
