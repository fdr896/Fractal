#include "complex.h"

const ld INF = 4;
const ld EPS = (ld)1e-8;
const int MAX = 256;

ld Complex::modulo2() {
    return this->re * this->re + this->im * this->im;
}

int Complex::get_Mandelbrot() {
    int iterations = 0;
    Complex z = Complex(0, 0);

    while ((z.modulo2() - INF) < EPS && iterations < MAX) {
        ++iterations;
        z = z * z + *this;
    }

    return iterations;
}

int Complex::get_Julia(Complex & c) {
    int iterations = 0;
    Complex z = *this;

    while ((z.modulo2() - INF) < EPS && iterations < 256) {
        ++iterations;
        z = z * z + c;
    }

    return iterations;
}
