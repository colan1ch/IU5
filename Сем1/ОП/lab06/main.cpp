#include "header.h"
using namespace std;


typedef double (*TPF) (double);


int main() {
    double a = -1, b = 3;
    I_Print res[4];
    TPF funcs[4] = {f1, f2, f3, f4};
    char *names[4] = {"y=x", "y=sin(22x)", "y=x^4", "arctg(x)"};
    double epses[5] = {0.01, 0.001, 0.0001, 0.00001, 0.000001};
    double real_values[4] = {(b*b - a*a) / 2, (cos(22*a) - cos(22*b)) / 22, (pow(b, 5) - pow(a, 5)) / 5,
                             b * atan(b) - a * atan(a) - (log(b*b + 1) - log(a*a + 1)) / 2};
    cout << "INT RECT\n";
    for (int j = 0; j < 5; j++){
        for (int i = 0; i < 4; i++){
            res[i].n = 0;
            res[i].name = names[i];
            res[i].i_toch = real_values[i];
            res[i].i_sum = IntRect(funcs[i], a, b, epses[j], res[i].n);
        }
        cout << "Погрешность eps = " << epses[j] << '\n';
        PrintTabl(res, 4);
        cout << "\n\n";
    }

    cout << "INT_TRAP\n";
    for (int j = 0; j < 5; j++){
        for (int i = 0; i < 4; i++){
            res[i].n = 0;
            res[i].name = names[i];
            res[i].i_toch = real_values[i];
            res[i].i_sum = IntTrap(funcs[i], a, b, epses[j], res[i].n);
        }
        cout << "Погрешность eps = " << epses[j] << '\n';
        PrintTabl(res, 4);
        cout << "\n\n";
    }
    return 0;
}
