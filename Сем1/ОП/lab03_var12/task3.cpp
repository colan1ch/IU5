#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;


int main() {
    cout << setw(3) << "x" << setw(13) << "S(x)" << setw(14) << "Y(x)\n";
    for (double x = 0; x <= 1; x += 0.2){
        double res_real = cos(2 * x);
        double res_teylor = 1;
        double cur_val = 1;
        int cur_num = 1;
        double delta = 1;
        while (delta > 0.000001){
            cur_val = cur_val * (-4 * x * x) / ((2 * cur_num) * (2 * cur_num - 1));
            res_teylor += cur_val;
            cur_num++;
            delta = abs(res_real - res_teylor);
        }
        cout << setw(3) << x << " " << setw(12) << res_teylor << " " << setw(12) << res_real << endl;
    }
    return 0;
}
