#include <iostream>
#include <iomanip>

double eps = 0.000001;


void div_row(double *row, double d, int n) {
    for (int i = 0; i < n; i++){
        row[i] = row[i] / d;
    }
}

void sum_rows(double *row, const double *source_row, double k, int n) {
    for (int i = 0; i < n; i++) {
        row[i] = row[i] + source_row[i] * k;
    }
}

void reverse_matrix_calc(double **matrix, int n){
    for (int i = 0; i < n; i++) {
        if (abs(matrix[i][i]) < eps) {
            for (int j = i + 1; j < n; j++) {
                if (abs(matrix[j][i]) > eps) {
                    std::swap(matrix[i], matrix[j]);
                    break;
                }
            }
        }
        if ((matrix[i][i] < (1 - eps)) || (matrix[i][i] > (1 + eps))) {
            div_row(matrix[i], matrix[i][i], n);
        }
        for (int j = i + 1; j < n; j++) {
            sum_rows(matrix[j], matrix[i], -matrix[j][i], n);
        }
    }

    for (int i = n - 1; i >= 0; i--) {
        for (int j = i - 1; j >= 0; j--) {
            sum_rows(matrix[j], matrix[i] ,-matrix[j][i], n);
        }
    }
}

int main() {
    int n;
    std::cout << "Введите размер матрицы\n";
    std::cin >> n;
    double **matrix = new double*[n];
    for (int i = 0; i < n; i++){
        matrix[i] = new double[2 * n];
    }
    std::cout << "Введите матрицу\n";
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            std::cin >> matrix[i][j];
            matrix[i][j + n] = (i == j) ? 1 : 0;
        }
    }
    reverse_matrix_calc(matrix, n);
    for (int i = 0; i < n; i++){
        for (int j = 0; j < 2 * n; j++){
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << '\n';
    }
    return 0;
}
