#include "mymath/mymath.h"
#include "lib2/library.h"

int main() {
    std::cout << "Я умею суммировать! Зацени 1+1=";
    std::cout << mymath::sum(10, 32) << std::endl;
    std::cout << mymath::inc(13) << std::endl;
    hello();
    return 0;
}
