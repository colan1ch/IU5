#include "MyStack/MyStack.h"
#include <iostream>


void multipliers(int n, MyStack<int> &stack) {
    int i = 2, cur = 1;
    if (n < 1) return;
    if (n == 1) {
        stack.push(1);
        return;
    }
    while (cur < n) {
        if ((n / cur) % i == 0) {
            cur *= i;
            stack.push(i);
        }
        else {
            i += 1;
        }
    }
}

int main() {
    MyStack<int> stack, stack2;
    int n = 3960;
    multipliers(n, stack);
    MyStack<int> a;
    a = stack;
    while (!a.empty()) {
        std::cout << a.top_inf();
        a.pop();
        if (!a.empty()) std::cout << ' ';
    }
    std::cout << '\n';
    std::cout << n << " = ";
    while (!stack.empty()) {
        std::cout << stack.top_inf();
        stack2.push(stack.top_inf());
        stack.pop();
        if (!stack.empty()) std::cout << " * ";
    }
    std::cout << '\n' << n << " = ";
    while (!stack2.empty()) {
        std::cout << stack2.top_inf();
        stack2.pop();
        if (!stack2.empty()) std::cout << " * ";
    }
    return 0;
}
