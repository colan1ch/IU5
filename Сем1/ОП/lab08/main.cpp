#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;


int factorial(int k){
    return (k == 0 || k == 1) ? 1 : factorial(k - 1) * k;
}


void filling_matrix(double **a, int N){
    int x = 1;
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            if (i < j){
                if (i == 0) {
                    a[i][j] = a[i][j - 1] / pow(j + 1, i + 1);
                }
                else{
                    a[i][j] = a[i - 1][j] * x / factorial(j + 1);
                }
            }
            else if (i > j) {
                if (j == 0) {
                    a[i][j] = (i == 1 ? -1 : 1) * a[i - 1][j] * (-x) / factorial(j + 1);
                }
                else{
                    a[i][j] = (i == 1 ? -1 : 1) * a[i][j - 1] / pow(j + 1, i + 1);
                }
            }
            else{
                a[i][j] = 1;
            }
        }
    }
}


template<typename T>
void print_matrix(T a, int N, int type, int acc){
    int cnt_in_line = min(160 / (acc + 8) + 1, N);
    if (type == 1){
        for (int i = 0; i < N; i++){
            for (int j = 0; j < cnt_in_line; j++){
                cout << setw(acc + 7) << fixed << setprecision(acc) << scientific << a[i][j] << ' ';
            }
            cout << '\n';
        }
        cout << '\n';
        for (int i = 0; i < N; i++){
            for (int j = cnt_in_line; j < N; j++){
                cout << setw(acc + 7) << fixed << setprecision(acc) << scientific << a[i][j] << ' ';
            }
            cout << '\n';
        }
    }
    if (type == 2) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < cnt_in_line; j++) {
                cout << setw(acc + 4) << fixed << setprecision(acc) << a[i][j] << ' ';
            }
            cout << '\n';
        }
        cout << '\n';
            for (int i = 0; i < N; i++) {
                if (cnt_in_line >= N) {
                    break;
                }
                for (int j = cnt_in_line; j < N; j++) {
                    cout << setw(acc + 4) << fixed << setprecision(acc) << a[i][j] << ' ';
                }
                cout << '\n';
            }
    }
}


int main() {
    int N;
    cout << "Введите N - размер матрицы\n";
    cin >> N;
    double **a = new double* [N];
    for (int i = 0; i < N; i++){
        a[i] = new double[N];
    }
    filling_matrix(a, N);
    int type, acc;
    cout << "Введите 1(научный вывод) или 2(вывод с фикс точкой)\n";
    cin >> type;
    cout << "Введите точность значений\n";
    cin >> acc;
    print_matrix(a, N, type, acc);
    double b[10][10];
    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            b[i][j] = i * 10 + j;
        }
    }
    cout << "Введите 1(научный вывод) или 2(вывод с фикс точкой)\n";
//    cout<<b<<endl;
//    cout<<"  "<<b[0]<<"  "<<b[6]<<endl;
//    cout<<b[3][0]<<"  "<<**b+4 <<"  "<<*b[7]<<endl;
//    cout<<*(*(b+2))<<"  "<<*b[1]<<endl;
//    cout<<*(b[6]+1)<<"  " <<*(*b+2)<<endl;
//    cout<<b[0][9]<<"  "<<*(b[0]+30)<<"  "<<*b[9]<<endl;
    cin >> type;
    cout << "Введите точность значений\n";
    cin >> acc;
    print_matrix(b, 10, type, acc);
    for (int i = 0; i < N; i++) {
        delete[] a[i];
    }
    delete[] a;
    return 0;
}
