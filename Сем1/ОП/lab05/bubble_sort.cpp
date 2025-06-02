#include <iostream>
using namespace std;


void bubble_sort(int* a, int N, bool is_reversed = false){
    int count_perm = 0, count_comp = 0, k;
    if (is_reversed)
        k = -1;
    else
        k = 1;
    for (int i = 0; i < N; i++){
        cout << a[i] << ' ';
    }
    cout << '\n';
    for (int i = 0; i < N - 1; i++){
        for (int j = 0; j < N - 1 - i; j++){
            if (k * a[j] > k * a[j + 1]){
                count_perm++;
                swap(a[j], a[j + 1]);
            }
            count_comp++;
        }
    }
    cout << "Метод пузырькового всплытия\n";
    cout << "Количество сравнений: " << count_comp << '\n';
    cout << "Количество перестановок: " << count_perm << "\n";
    for (int i = 0; i < N; i++){
        cout << a[i] << ' ';
    }
    cout << '\n';
}