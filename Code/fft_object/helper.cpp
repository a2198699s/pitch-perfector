#include <iostream>
#include "helper.h"
#include <cmath>

const int A4 = 440.0;

double average(double* inputArray, int size) {
    double total = 0;
    // int size = sizeof(inputArray)/sizeof(inputArray[0]);
    for (int i=0; i<size; ++i) {
        total += inputArray[i];
    }
    return total / size;
}

double getFrequencyAbove(double frequency) {
    return( A4 * pow( (pow( 2, (1.0/12.0) )), 1));
}

double getFrequencyBelow(double frequency) {
    return( A4 * pow( (pow( 2, (1.0/12.0) )), -1));
}
