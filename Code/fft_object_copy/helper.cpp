#include <iostream>
#include "helper.h"


double average(double* inputArray, int size) {
    double total = 0;
    // int size = sizeof(inputArray)/sizeof(inputArray[0]);
    for (int i=0; i<size; ++i) {
        total += inputArray[i];
    }
    return total / size;
}