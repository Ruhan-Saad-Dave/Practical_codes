#include <iostream>
#include <graphics.h>
using namespace std;

class HilbertCurve {
public:
    // Recursive function to draw Hilbert Curve
    void drawHilbertCurve(int x, int y, int xi, int xj, int yi, int yj, int n) {
        if (n <= 0) {
            int xEnd = x + (xi + yi) / 2;
            int yEnd = y + (xj + yj) / 2;
            line(x, y, xEnd, yEnd);
            return;
        }
        drawHilbertCurve(x, y, yi / 2, yj / 2, xi / 2, xj / 2, n - 1);
        drawHilbertCurve(x + xi / 2, y + xj / 2, xi / 2, xj / 2, yi / 2, yj / 2, n - 1);
        drawHilbertCurve(x + xi / 2 + yi / 2, y + xj / 2 + yj / 2, xi / 2, xj / 2, yi / 2, yj / 2, n - 1);
        drawHilbertCurve(x + xi / 2 + yi, y + xj / 2 + yj, -yi / 2, -yj / 2, -xi / 2, -xj / 2, n - 1);
    }

    void centerHilbertCurve(int &x, int &y, int &length) {
        int screenWidth = getmaxx();
        int screenHeight = getmaxy();
        int centerX = screenWidth / 2;
        int centerY = screenHeight / 2;
        length = min(screenWidth, screenHeight) / 2; // Scale to fit the screen
        x = centerX - length / 2;
        y = centerY - length / 2;
    }
};

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, NULL);

    HilbertCurve hilbertCurve;
    int x, y, length, iterations;

    hilbertCurve.centerHilbertCurve(x, y, length);
    cout << "Enter the number of iterations for Hilbert Curve: ";
    cin >> iterations;

    hilbertCurve.drawHilbertCurve(x, y, length, 0, 0, length, iterations);

    getch();
    closegraph();
    return 0;
}
