#ifndef COMPLEX_H
#define COMPLEX_H

using ld = long double;

class Complex {
public:
    ld re, im;

    Complex() : re(0), im(0) {
    }

    Complex(ld _re, ld _im) : re(_re), im(_im) {
    }

    Complex operator + (Complex & other) {
        return Complex(re + other.re, im + other.im);
    }

    Complex operator - (Complex & other) {
        return Complex(re - other.re, im - other.im);
    }

    Complex operator * (Complex & other) {
        return Complex(re * other.re - im * other.im, re * other.im + im * other.re);
    }

    ld modulo2();

    int get_Mandelbrot();
    int get_Julia(Complex & c);
};

#endif // COMPLEX_H
