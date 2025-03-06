#include <iostream>

void f(int i) {
    int *p, y;
    p = new int[10];  
    if (i) p = &y;  
    delete[] p;
}
