#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;


int main(){
    cout << setw(10) << "x" << setw(13) << "S(x)" << setw(13) << "Y(x)" << setw(6) << "N\n";
    cout << fixed << setprecision(8);
    double res_real, res_teylor, cur_res, eps = 0.000001;
    int cur_num;
    for (double x = 0; x <= 0.8; x += 0.2) {
        res_real = log((1 + x) / (1 - x));
        res_teylor = x;
        cur_num = 1;
        cur_res = x;
        while (cur_res > eps){
            cur_res = cur_res * x * x * (2 * cur_num - 1) / (2 * cur_num + 1);
            cur_num++;
            res_teylor += cur_res;
        }
        res_teylor *= 2;
        cout << setw(3) << x << setw(13) << res_teylor << setw(13) << res_real << setw(5) << cur_num << endl;
    }
    return 0;
}
