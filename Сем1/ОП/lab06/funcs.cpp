#include <cmath>
#include <iostream>
#include <iomanip>
using namespace std;

typedef double (*TPF) (double);

double f1(double x){
    return x;
}

double f2(double x){
    return sin(22 * x);
}

double f3(double x){
    return pow(x, 4);
}

double f4(double x){
    return atan(x);
}


struct I_Print{
    char* name;
    double i_sum;
    double i_toch;
    int n;
};

void PrintTabl(I_Print i_prn[],int k){
    const int m=4;//число столбцов таблицы
    int wn[m]={12,18,18,10};//ширина столбцов таблицы
    char *title[m]={"Function","Integral","IntSum","N "};
    int size[m];
    for(int i=0;i<m;i++)
        size[i]=strlen(title[i]);
//шапка таблицы
    cout<<char(218)<<setfill(char(196));
    for(int j=0;j<m-1;j++)
        cout<<setw(wn[j])<<char(194);
    cout<<setw(wn[m-1])<<char(191)<<endl;

    cout<<char(179);
    for(int j=0;j<m;j++)
        cout<<setw((wn[j]-size[j])/2)<<setfill(' ')<<' '<<title[j]
            <<setw((wn[j]-size[j])/2)<<char(179);
    cout<<endl;
    for(int i=0;i<k;i++)
    {//заполнение таблицы
        cout<<char(195)<<fixed;
        for(int j=0;j<m-1;j++)
            cout<<setfill(char(196))<<setw(wn[j])<<char(197);
        cout<<setw(wn[m-1])<<char(180)<<setfill(' ')<<endl;

        cout<<char(179)<<setw((wn[0]-strlen(i_prn[i].name))/2)<<' '<<i_prn[i].name
            <<setw((wn[0]-strlen(i_prn[i].name))/2)<<char(179);
        cout<<setw(wn[1]-1)<<setprecision(10)<<i_prn[i].i_toch<<char(179)
            <<setw(wn[2]-1)<<i_prn[i].i_sum<<setprecision(6)<<char(179)
            <<setw(wn[3]-1)<<i_prn[i].n<<char(179)<<endl;
    }
//низ таблицы
    cout<<char(192)<<setfill(char(196));
    for(int j=0;j<m-1;j++)
        cout<<setw(wn[j])<<char(193);
    cout<<setw(wn[m-1])<<char(217)<<setfill(' ')<<endl;
}


double IntRect(TPF f, double a, double b, double eps, int& n){
    double dx = b - a;
    double s_prev, s = f(a + dx / 2) * dx;
    double r = 1;
    n = 1;
    while (r > eps){
        dx /= 2;
        n *= 2;
        s_prev = s;
        s = 0;
        for (int i = 1; i <= n; i++){
            s = s + f(a + dx / 2 + (i - 1) * dx);
        }
        s *= dx;
        r = abs(s_prev - s) / 3;
    }
    return s;
}


double IntTrap(TPF f, double a, double b, double eps, int& n){
    double dx = b - a;
    double x = a;
    double s_prev, s = dx * (f(x) + f(x + dx)) / 2;
    double r = 1;
    n = 1;
    while (r > eps){
        x = a;
        dx /= 2;
        n *= 2;
        s_prev = s;
        s = 0;
        for (int i = 1; i <= n; i++){
            s = s + (f(x) + f(x + dx)) / 2;
            x += dx;
        }
        s *= dx;
        r = abs(s_prev - s) / 3;
    }
    return s;
}