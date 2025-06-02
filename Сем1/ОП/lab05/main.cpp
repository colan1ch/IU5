#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "sorts.h"
using namespace std;


int iRandom(int a, int b){
    return a + rand() % (b - a);
}


void min_sort(int* a, int N, bool is_reversed = false){
    int count_perm = 0, count_comp = 0, min_ind, k;
    if (is_reversed)
        k = -1;
    else
        k = 1;
    for (int i = 0; i < N; i++){
        cout << a[i] << ' ';
    }
    cout << '\n';
    for (int i = 0; i < N; i++){
        min_ind = i;
        for (int j = i + 1; j < N; j++){
            if (k * a[j] < k * a[min_ind]) min_ind = j;
            count_comp++;
        }
        swap(a[i], a[min_ind]);
        if (i != min_ind) count_perm++;
    }
    cout << "Метод выбора минимального элемента\n";
    cout << "Количество сравнений: " << count_comp << '\n';
    cout << "Количество перестановок: " << count_perm << "\n";
    for (int i = 0; i < N; i++){
        cout << a[i] << ' ';
    }
    cout << '\n';
}


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


int main(){
    int N;
    cin >> N;
    int *arr = new int[N];
    int *arr2 = new int[N];
    srand(time(0));
    rand();
    for (int i = 0; i < N; i++){
        arr[i] = iRandom(1, 100);
        arr2[i] = arr[i];
    }

    min_sort(arr, N);
    cout << '\n';
    bubble_sort(arr2, N);
    cout << '\n';

    min_sort(arr, N);
    cout << '\n';
    bubble_sort(arr2, N);
    cout << '\n';

    min_sort(arr, N, true);
    cout << '\n';
    bubble_sort(arr2, N, true);
    cout << '\n';
    delete[] arr;
    delete[] arr2;
    return 0;
}
