#include <iostream>
using namespace std;

bool isPovt(int **a, int n, int k) {
    int cnt = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (a[i][j] == k) cnt++;
        }
    }
    if (cnt > 1) return true;
    return false;
}

void fillingMatrix(int **a, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
//            a[i][j] = rand() % 200 - 100;
            cin >> a[i][j];
        }
    }
}

void fillingMas(int *c, int m) {
    for (int i = 0; i < m; i++) {
//        c[i] = rand() % 200 - 100;
        cin >> c[i];
    }
}


void task1() {
    int n;
    cin >> n;
    int **a = new int *[n];
    for (int i = 0; i < n; i++) a[i] = new int[n];
    fillingMatrix(a, n);
    int ma = -9999;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if ((isPovt(a, n, a[i][j])) && (a[i][j] > ma)) {
                ma = a[i][j];
            }
        }
    }
    cout << "Answer for task 1: " << ma << '\n';
    for (int i = 0; i < n; i++) delete[] a[i];
    delete[] a;
}


void task2(int m) {
    int *c = new int[m];
    int cnt = 0;
    fillingMas(c, m);
    for (int i = 0; i < m; i++) {
        if (c[i] == m) cnt++;
    }
    cout << "Answer for task 2: " << cnt << '\n';
    delete[] c;
}


void task3() {
    char *s = new char[1000];
    cin.getline(s, 1000);
    int cnt = 0;
    if (s[0] == 'P') cnt++;
    for (int i = 0; i < strlen(s) - 1; i++) {
        if (s[i] == ' ' && s[i + 1] == 'P') cnt++;
    }
    cout << "Answer for task 3: " << cnt << '\n';
    delete[] s;
}


int main(int argc, char **argv) {
    if (argc != 2) {
        return 0;
    }
    int m = atoi(argv[1]);
    task3();
    task1();
    task2(m);
    return 0;
}
