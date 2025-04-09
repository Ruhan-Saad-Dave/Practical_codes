#include <iostream>

using namespace std;

class Complex {
private:
    double real;
    double imag;

public:
    Complex() : real(0.0), imag(0.0) {}

    Complex(double real, double imag) : real(real), imag(imag) {}

    // Overloaded operator+ to add two complex numbers
    Complex operator+(const Complex& other) const {
        return Complex(real + other.real, imag + other.imag);
    }

    // Overloaded operator* to multiply two complex numbers
    Complex operator*(const Complex& other) const {
        double newReal = (real * other.real) - (imag * other.imag);
        double newImag = (real * other.imag) + (imag * other.real);
        return Complex(newReal, newImag);
    }

    // Overloaded << to print Complex Numbers
    friend ostream& operator<<(ostream& out, const Complex& c) {
        out << c.real;
        if (c.imag >= 0)
            out << "+";
        out << c.imag << "i";
        return out;
    }

    // Overloaded >> to read Complex Numbers
    friend istream& operator>>(istream& in, Complex& c) {
        in >> c.real;
        in.ignore(); // Ignore the '+' or '-'
        in >> c.imag;
        in.ignore(); // Ignore the 'i'
        return in;
    }
};

int main() {
    Complex c1, c2, c3;
    
    cout << "Enter the first complex number (real+imagi): ";
    cin >> c1;
    cout << "Enter the second complex number (real+imagi): ";
    cin >> c2;
    
    c3 = c1 + c2;
    cout << "Sum of the complex numbers: " << c3 << endl;
    
    c3 = c1 * c2;
    cout << "Product of the complex numbers: " << c3 << endl;
    
    return 0;
}
