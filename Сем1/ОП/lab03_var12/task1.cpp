#include <iostream>
using namespace std;


int main() {
    int n, m, sum;
    char cmd;
    do{
        sum = 0;
        cin >> n >> m;
        for (int i = 1; i <= n; i++){
            if ((i % 5 == 0) && (i % m != 0)) sum += i;
        }
        cout << sum << endl;
        cout << "Продолжить работу? y/n" << endl;
        cin >> cmd;
    } while (cmd != 'n');
    return 0;
}
