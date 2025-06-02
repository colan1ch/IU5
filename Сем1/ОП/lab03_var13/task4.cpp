#include <iostream>
using namespace std;


int main() {
    int n, x;
    char cmd;
    double cur_val, cur_sum;
    do {
        cout << "Введите n и x\n";
        cin >> n >> x;
        cur_val = x;
        cur_sum = x;
        for (int i = 1; i <= n; i++) {
            if ((i == 3) || (i == 5) || (i == 10)) {
                cout << i << " слагаемых: " << cur_sum << endl;
            }
            if (i == 1)
                cur_val = cur_val * x * i / (i * 2);
            else
                cur_val = cur_val * x * (i - 1) / (i * 2);
            cur_sum += cur_val;
        }
        cout << cur_sum << endl;
        cout << "Желаете продолжить? y/n\n";
        cin >> cmd;
    } while (cmd != 'n');
    return 0;
}
