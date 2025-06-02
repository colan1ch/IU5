#include "header.h"

int main(){
    double eps1 = 0.000001, eps2 = 0.00000001;
    int k1 = 1, k2 = 10;
    std::cout << "eps = " << eps1 << ", k = " << k1 << '\n';
    iterMethod(eps1, k1);
    binSearch(eps1, k1);
    NewtonMethod(eps1, k1);
    std::cout << "eps = " << eps2 << ", k = " << k1 << '\n';
    iterMethod(eps2, k1);
    binSearch(eps2, k1);
    NewtonMethod(eps2, k1);
    std::cout << "eps = " << eps1 << ", k = " << k2<< '\n';
    iterMethod(eps1, k2);
    binSearch(eps1, k2);
    NewtonMethod(eps1, k2);
    return 0;
}
