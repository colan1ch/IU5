#include <iostream>
using namespace std;


int main() {
    int n, x;
    char cmd;
    do {
        cin >> n >> x;
        double cur_val = 1;
        double cur_sum = 1;
        for (int i = 1; i <= n; i++) {
            if ((i == 3) || (i == 5) || (i == 10)) {
                cout << i << " слагаемых: " << cur_sum << endl;
            }
            cur_val = cur_val * (x * x) / ((2 * i) * (2 * i - 1));
            cur_sum += cur_val;
        }
        cout << cur_sum << endl;
        cout << "Желаете продолжить? y/n\n";
        cin >> cmd;
    } while (cmd != 'n');
    return 0;
}
