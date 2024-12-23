

#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define M_PI_4 (M_PI / 4)

// Funkcija za izračun arctan
double calcAtan(double* x, int* N_steps) {
    double value = *x / 2; 
    double result = value;
    double term = value;

    for (int n = 1; n < *N_steps; ++n) {
        term *= -(*x * *x) / (4 * n * n + 2 * n); 
        result += term / (2 * n + 1); 
    }

    return result;
}

// trapezna metoda
double trapezoidalIntegral(double a, double b, int n, int steps) {
    double dx = (b - a) / n;
    double sum = 0.0;

    for (int i = 0; i <= n; ++i) {
        double x = a + i * dx;
        double f = std::exp(3 * x) * calcAtan(&x, &steps);
        if (i == 0 || i == n) {
            sum += f; 
        }
        else {
            sum += 2 * f; 
        }
    }

    return (dx / 2) * sum;
}

int main() {
    double a = 0.0;
    double b = M_PI_4; 
    int n = 1000; // Število podintervalov 
    int steps = 50; // Število členov Taylorjeve vrste 

    double result = trapezoidalIntegral(a, b, n, steps);

    std::cout << "Približna vrednost integrala je: " << result << std::endl;
    return 0;
}
