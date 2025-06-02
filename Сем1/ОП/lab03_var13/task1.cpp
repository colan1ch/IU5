#include <iostream>
using namespace std;


int main() {
    int n, m, sum;
    char cmd;
    do{
        sum = 0;
        cout << "Введите n и m\n";
        cin >> n >> m;
        for (int i = 0; i <= n; i += 5){
            if (i % m != 0) sum += i;
        }
        cout << sum << endl;
        cout << "Продолжить работу? y/n" << endl;
        cin >> cmd;
    } while (cmd != 'n');
    return 0;
}
