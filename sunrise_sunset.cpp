#include <GL/glut.h>
#include <cmath>

float sunPosition = 0.0;

void drawSun() {
    glColor3f(1.0, 1.0, 0.0); // Yellow color for the sun
    glTranslatef(0.0, sin(sunPosition) * 3.0, 0.0); // Move sun up and down
    glutSolidSphere(1.0, 30, 30); // Draw a simple sun
}

void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Draw sky
    glColor3f(0.0, 0.7, 1.0); // Blue color for the sky
    glBegin(GL_QUADS);
    glVertex2f(-10.0, -10.0);
    glVertex2f(10.0, -10.0);
    glVertex2f(10.0, 10.0);
    glVertex2f(-10.0, 10.0);
    glEnd();

    // Draw ground
    glColor3f(0.0, 0.5, 0.0); // Green color for the ground
    glBegin(GL_QUADS);
    glVertex2f(-10.0, -10.0);
    glVertex2f(10.0, -10.0);
    glVertex2f(10.0, -5.0);
    glVertex2f(-10.0, -5.0);
    glEnd();

    // Draw sun
    drawSun();

    glutSwapBuffers();
}

void update(int value) {
    sunPosition += 0.02; // Increment sun position for animation

    glutPostRedisplay(); // Request a redraw
    glutTimerFunc(16, update, 0); // 60 frames per second
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Sunrise and Sunset");
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(drawScene);
    glutTimerFunc(25, update, 0);

    glutMainLoop();
    return 0;
}