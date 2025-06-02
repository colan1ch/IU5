#include "docs/docs.h"


Document **Document::docs = new Document*[1];
int Document::size = 0;
int Document::capacity = 1;


int main() {
    Receipt r(123, "receipt mem", 120, "Coca-cola");
    Check c(1, "check pepsi", 80, "Pepsi-cola", 20);
    Invoice i(5, "invoice for delivery TV", 50000, "LG 4K", 3000);
    Document::print();
    delete[] Document::docs;
    return 0;
}
