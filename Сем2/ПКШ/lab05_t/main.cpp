//#include "VecnSet/MyVector.h"
//#include "VecnSet/MySet.h"
//
//
////int main() {
////    std::cout << "Check MySet<int>\n";
////    MySet<int> s1i, s2i(4);
////    s1i.add_element(3);
////    s1i.add_element(1);
////    s1i.add_element(5);
////    s1i.delete_element(5);
////    s2i.add_element(2);
////    s2i.add_element(2);
////    std::cout << s1i << s2i;
////    MySet<int> plusi = s1i + s2i, minusi(1000);
////    minusi = s1i - s2i;
////    s1i *= s2i;
////    std::cout << plusi << minusi << s1i;
////
////    std::cout << "Check MySet<char *>\n";
////    MySet<char *> s1, s2("4");
////    s1.add_element("3");
////    s1.add_element("1");
////    s1.add_element("5");
////    s1.delete_element("5");
////    s2.add_element("2");
////    s2.add_element("2");
////    s2.add_element("qwe");
////    std::cout << s1 << s2;
////    MySet<char *> plus = s1 + s2, minus("1000");
////    minus = s1 - s2;
////    s1 *= s2;
////    s1 = s2;
////    std::cout << plus << minus << s1 << '\n';
////
////    std::cout << "Check MyVector<int>: \n";
////    MyVector<int> v1;
////    v1.add_element(1);
////    v1.add_element(5);
////    v1.add_element(2);
////    v1.add_element(3);
////    MyVector<int> v2(v1);
////    v2.sort();
////    v2.delete_element(3);
////    std::cout << v1 << v2;
////    MyVector<int> v3(5);
////    v3 = v1;
////    v3.add_element(123);
////    std::cout << v3 << "index of val 123: " << v3.find(123);
////    std::cout << "\nv3 length: " << v3.len();
////
////    std::cout << "\n\nCheck MyVector<char *>: \n";
////    MyVector<char *> c1;
////    c1.add_element("1");
////    c1.add_element("5");
////    c1.add_element("2");
////    c1.add_element("qwe");
////    MyVector<char *> c2(c1);
////    c2.sort();
////    c2.delete_element(3);
////    std::cout << c1 << c2;
////    MyVector<char *> c3("5");
////    c3 = c1;
////    c3.add_element("123");
////    std::cout << c3 << "index of val 123: " << c3.find("123");
////    std::cout << "\nv3 length: " << c3.len();
////    return 0;
////}

#include "VecnSet/MySet.h"

#define MAX_SIZE 3

int main() {
    MyVector<std::string> v("Hello!");
    v.add_element("Привет!");
    v.add_element("Привет!");
    v.add_element("Привет!");
    v.add_element("Привет!");
    v.add_element("Привет!");
    std::cout << "Вектор v: " << v << std::endl;
    v.add_element("Привет!");
    v.add_element("Привет!");
    v.add_element("Привет!");
    std::cout << "Вектор v: " << v << std::endl;
    MyVector v1 = v;
    std::cout << "Вектор v1: " << v1 << std::endl;
    for (int i = 0; i < MAX_SIZE; i++) {
        v1.delete_element(0);
    }
    std::cout << "Вектор v1: " << v1 << std::endl;
    MySet<std::string> s("Yes"), s1, s2;
    s.add_element("Привет!");
    s.add_element("No");
    char *str = "Hello!";
    s.add_element(str);
    std::cout << "Множество s: " << s << std::endl;
    s1.add_element("Cat");
    s1.add_element("No");
    s1.add_element("Привет!");
    std::cout << "Множество s1: " << s1 << std::endl;
    s2 = s1 - s;
    std::cout << "Множество s2=s1-s: " << s2 << std::endl;
    std::cout << "Множество s1: " << s1 << std::endl;
    std::cout << "Множество s: " << s << std::endl;
    s2 = s - s1;
    std::cout << "Множество s2=s-s1: " << s2 << std::endl;
    std::cout << "Множество s1: " << s1 << std::endl;
    std::cout << "Множество s: " << s << std::endl;
    s2 = s1 + s;
    std::cout << "Множество s2=s1+s: " << s2 << std::endl;
    std::cout << "Множество s1: " << s1 << std::endl;
    std::cout << "Множество s: " << s << std::endl;
    s2 = s1 * s;
    std::cout << "Множество s2=s1*s: " << s2 << std::endl;
    std::cout << "Множество s1: " << s1 << std::endl;
    std::cout << "Множество s: " << s << std::endl;
    MySet s3 = s2;
    std::cout << "Множество s3=s2: " << s3 << std::endl;
    if (s3 == s2)
        std::cout << "Множество s3=s2\n";
    else
        std::cout << "Множество s3!=s2\n";
    if (s3 == s1)
        std::cout << "Множество s3=s1\n";
    else
        std::cout << "Множество s3!=s1\n";
    if (s1 == s3)
        std::cout << "Множество s1=s3\n";
    else
        std::cout << "Множество s1!=s3\n";
    return 0;
}


// #include "VecnSet/MySet.h"
//
//
// int main() {
//     MySet<std::string> s;
//     s.add_element(std::string("qwe"));
//     s.add_element(std::string("asd"));
//     s.add_element(std::string("xc"));
//     s.add_element(std::string("zxczxc"));
//     std::cout << s;
// }
