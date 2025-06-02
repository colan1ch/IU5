#include <time.h>
#include <stdlib.h>
#include <iostream>
#include "sorts.h"
using namespace std;


int iRandom(int a, int b){
    return a + rand() % (b - a);
}


void print(int* a, int N){
    for (int i = 0; i < N; i++) cout << a[i] << ' ';
    cout << '\n';
}


int main(){
    int N;
    cout << "Введите N - длину массива\n";
    cin >> N;
    int *arr = new int[N];
    int *arr2 = new int[N];
    srand(time(0));
    rand();
    for (int i = 0; i < N; i++){
        arr[i] = iRandom(1, 100);
        arr2[i] = arr[i];
    }

    print(arr, N);
    min_sort(arr, N);
    print(arr, N);
    cout << '\n';
    bubble_sort(arr2, N);
    print(arr2, N);
    cout << '\n';

    print(arr, N);
    min_sort(arr, N);
    print(arr, N);
    cout << '\n';
    bubble_sort(arr2, N);
    print(arr2, N);
    cout << '\n';

    print(arr, N);
    min_sort(arr, N, -1);
    print(arr, N);
    cout << '\n';
    bubble_sort(arr2, N, -1);
    print(arr2, N);
    cout << '\n';
    delete[] arr;
    delete[] arr2;

    int *arr5 = new int[5];
    int *arr5_2 = new int[5];
    int *arr50 = new int[50];
    int *arr50_2 = new int[50];
    int *arr500 = new int[500];
    int *arr500_2 = new int[500];
    for (int i = 0; i < 5; i++) {
        arr5[i] = iRandom(1, 100);
        arr5_2[i] = arr5[i];
    }
    for (int i = 0; i < 50; i++) {
        arr50[i] = iRandom(1, 100);
        arr50_2[i] = arr50[i];
    }
    for (int i = 0; i < 500; i++) {
        arr500[i] = iRandom(1, 100);
        arr500_2[i] = arr500[i];
    }

    cout << "5 элементов: ";
    min_sort(arr5, 5);
    cout << "\n5 элементов: ";
    bubble_sort(arr5_2, 5);
    cout << '\n';

    cout << "50 элементов: ";
    min_sort(arr50, 50);
    cout << "\n50 элементов: ";
    bubble_sort(arr50_2, 50);
    cout << '\n';

    cout << "500 элементов: ";
    min_sort(arr500, 500);
    cout << "\n500 элементов: ";
    bubble_sort(arr500_2, 500);
    cout << '\n';

    delete[] arr5;
    delete[] arr50;
    delete[] arr500;
    delete[] arr5_2;
    delete[] arr50_2;
    delete[] arr500_2;
    return 0;
}
