#include <curses.h>
#include <stdio.h>

int main() {
    char c;
    c = getch();
    while (c != '.'){
        if ((c >= 'a' && c <= 'z') || (c == ' ')){
            putchar(int(c));
        }
        c = getch();
    }
    return 0;
}
