/*
Write a C++ program to implement bouncing ball using sine wave form. Apply the concept of polymorphism.
*/

#include <GL/glut.h>
#include <cmath>
#include <iostream>

// Base class for a Ball
class Ball {
protected:
    float x;          // x position of the ball
    float amplitude;  // Amplitude of the sine wave
    float frequency;  // Frequency of the sine wave
    float time;       // Time variable for animation

public:
    Ball(float x, float amplitude, float frequency)
        : x(x), amplitude(amplitude), frequency(frequency), time(0) {}

    virtual void move() = 0; // Pure virtual function for moving the ball
    virtual void draw() = 0;  // Pure virtual function for drawing the ball
};

// Derived class for SineWaveBall
class SineWaveBall : public Ball {
public:
    SineWaveBall(float x, float amplitude, float frequency)
        : Ball(x, amplitude, frequency) {}

    // Override the move function to implement bouncing behavior
    void move() override {
        time += 0.1f; // Increment time
    }

    // Override the draw function to draw the ball at its current position
    void draw() override {
        float y = amplitude * sin(frequency * time); // Calculate y using sine wave
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(1.0f, 0.0f, 0.0f); // Set ball color to red
        glVertex2f(x, y); // Center of the ball

        // Create the ball shape
        for (int i = 0; i <= 20; i++) {
            float angle = 2.0f * M_PI * float(i) / float(20); // Circle angles
            float dx = 0.1f * cos(angle); // x offset
            float dy = 0.1f * sin(angle); // y offset
            glVertex2f(x + dx, y + dy);
        }
        glEnd();
    }
};

// Global variables
SineWaveBall *ball;

// Function to initialize OpenGL
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Set the clear color to black
    glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D
}

// Display function for rendering
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen
    glLoadIdentity();

    // Draw the bouncing ball
    ball->move(); // Update ball position
    ball->draw(); // Draw the ball

    glutSwapBuffers(); // Swap buffers for smooth animation
}

// Timer function for animation
void timer(int value) {
    glutPostRedisplay(); // Request to redraw the scene
    glutTimerFunc(16, timer, 0); // Call timer again after 16 ms
}

// Main function
int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Bouncing Ball with Sine Wave");

    init();
    ball = new SineWaveBall(0.0f, 1.0f, 1.0f); // Create a sine wave ball

    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0); // Start the timer for animation
    glutMainLoop(); // Enter the GLUT event processing loop

    delete ball; // Clean up
    return 0;
}
