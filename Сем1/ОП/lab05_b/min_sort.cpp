#include <iostream>
using namespace std;


void min_sort(int* a, int N, int k = 1){
    int count_perm = 0, count_comp = 0, min_ind;
    for (int i = 0; i < N; i++){
        min_ind = i;
        for (int j = i + 1; j < N; j++){
            if (k * a[j] < k * a[min_ind]) {
                min_ind = j;
            }
            count_comp++;
        }
        if (i != min_ind) {
            swap(a[i], a[min_ind]);
            count_perm++;
        }
    }
    cout << "Метод выбора минимального элемента\n";
    cout << "Количество сравнений: " << count_comp << '\n';
    cout << "Количество перестановок: " << count_perm << '\n';
}
