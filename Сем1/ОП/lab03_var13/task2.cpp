#include <iostream>
#include <iomanip>
using namespace std;


int main() {
    double a, ans = 1.0;
    cout << "Введите а\n";
    cin >> a;
    if (a >= 0){
        for (int i = 2; i <= 8; i += 2) ans *= i * i;
        ans -= a;
    }
    else{
        for (int i = 3; i <= 9; i += 3) ans *= i - 2;
    }
    cout << fixed << setprecision(6) << ans;
    return 0;
}
