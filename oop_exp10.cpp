/*
Write C++ program to generate fractal patternns by using koch curves
*/

#include <GL/glut.h>
#include <cmath>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
int depth = 4; // Depth of the Koch curve, adjust this for more complexity

// Function to draw a single Koch curve recursively
void drawKochCurve(float x1, float y1, float x2, float y2, int depth) {
    if (depth == 0) {
        // Draw the line segment if at the base case
        glBegin(GL_LINES);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();
    } else {
        // Calculate the points to form a Koch curve segment
        float dx = (x2 - x1) / 3.0;
        float dy = (y2 - y1) / 3.0;

        // First point (1/3 along the way)
        float xA = x1 + dx;
        float yA = y1 + dy;

        // Second point (2/3 along the way)
        float xB = x1 + 2 * dx;
        float yB = y1 + 2 * dy;

        // Calculate the peak of the equilateral triangle
        float angle = M_PI / 3; // 60 degrees in radians
        float xPeak = xA + dx * cos(angle) - dy * sin(angle);
        float yPeak = yA + dx * sin(angle) + dy * cos(angle);

        // Recursive calls to draw each segment of the Koch curve
        drawKochCurve(x1, y1, xA, yA, depth - 1);  // Left segment
        drawKochCurve(xA, yA, xPeak, yPeak, depth - 1); // Triangle peak
        drawKochCurve(xPeak, yPeak, xB, yB, depth - 1); // Right side of peak
        drawKochCurve(xB, yB, x2, y2, depth - 1); // Right segment
    }
}

// Function to display the Koch curve fractal
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Set the line color to white
    glColor3f(1.0f, 1.0f, 1.0f);

    // Initial points for the Koch curve (horizontal line)
    float startX = -0.5f;
    float startY = 0.0f;
    float endX = 0.5f;
    float endY = 0.0f;

    // Draw the Koch curve
    drawKochCurve(startX, startY, endX, endY, depth);

    glFlush(); // Flush the drawing pipeline to display the result
}

// Initialize the OpenGL environment
void initOpenGL() {
    glClearColor(0.0, 0.0, 0.0, 0.0); // Set background color to black
    glMatrixMode(GL_PROJECTION);       // Set up the projection matrix
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);  // Set the coordinate system for the window
}

// Main function to set up the OpenGL window and callbacks
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Koch Curve Fractal");

    initOpenGL();

    glutDisplayFunc(display); // Register the display callback function
    glutMainLoop();           // Enter the OpenGL main loop

    return 0;
}
