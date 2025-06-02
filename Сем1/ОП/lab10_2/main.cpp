#include <iostream>
#include <iomanip>
using namespace std;


void div_row(double *row, int n, double d) {
    for (int i = 0; i < 2 * n; i++) {
        row[i] /= d;
    }
}

void sum_rows(double *row, const double *source, int n, double k) {
    for (int i = 0; i < 2 * n; i++) {
        row[i] += source[i] * k;
    }
}

bool search_rev_mtx(double **mtx, int n) {
    bool is_rev_exist;
    for (int i = 0; i < n; i++) {
        if (mtx[i][i] == 0) {
            is_rev_exist = false;
            for (int j = i + 1; (j < n) && (!is_rev_exist); j++) {
                if (mtx[j][i] != 0) {
                    swap(mtx[i], mtx[j]);
                    is_rev_exist = true;
                }
            }
            if (!is_rev_exist) {
                return false;
            }
        }
        else if (mtx[i][i] != 1) {
            div_row(mtx[i], n, mtx[i][i]);
        }
        for (int j = 0; j < n; j++) {
            if (j != i) {
                sum_rows(mtx[j], mtx[i], n, -mtx[j][i]);
            }
        }
    }
    return true;
}


void slay_solving(double **mtx, double *free_coeffs, int n) {
    double *ans = new double[n];
    for (int i = 0; i < n; i++) {
        ans[i] = 0;
        for (int j = 0; j < n; j++) {
            ans[i] += mtx[i][j + n] * free_coeffs[j];
        }
    }
    for (int i = 0; i < n; i++) {
        cout << 'x' << i + 1 << " = " << ans[i] << '\n';
    }
    delete[] ans;
}

int main() {
    int n;
    cout << "Введите n - размер матрицы\n";
    cin >> n;
    double **mtx = new double *[n];
    for (int i = 0; i < n; i++) {
        mtx[i] = new double[2 * n];
    }
    cout << "Введите матрицу\n";
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> mtx[i][j];
            mtx[i][j + n] = (i == j) ? 1 : 0;
        }
    }
    if (search_rev_mtx(mtx, n)) {
        cout << "Обратная матрица:\n";
        for (int i = 0; i < n; i++) {
            for (int j = n; j < 2 * n; j++) {
                cout << setw(8) << setprecision(5) << mtx[i][j] << ' ';
            }
            cout << '\n';
        }
        cout << '\n';
        double *free_coeffs = new double[n];
        cout << "Введите свободные члены СЛАУ\n";
        for (int i = 0; i < n; i++) {
            cin >> free_coeffs[i];
        }
        slay_solving(mtx, free_coeffs, n);
        delete[] free_coeffs;
    }
    else {
        cout << "Обратной матрицы не существует\n";
    }
    for (int i = 0; i < n; i++) {
        delete[] mtx[i];
    }
    delete[] mtx;
    return 0;
}
