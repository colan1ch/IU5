#include <iostream>
using namespace std;


void bubble_sort(int* a, int N, int k = 1){
    int count_perm = 0, count_comp = 0, cur_cnt;
    for (int i = 0; i < N - 1; i++){
        cur_cnt = 0;
        for (int j = 0; j < N - 1 - i; j++){
            if (k * a[j] > k * a[j + 1]){
                count_perm++;
                swap(a[j], a[j + 1]);
                cur_cnt++;
            }
            count_comp++;
        }
        if (cur_cnt == 0) break;
    }
    cout << "Метод пузырькового всплытия\n";
    cout << "Количество сравнений: " << count_comp << '\n';
    cout << "Количество перестановок: " << count_perm << '\n';
}