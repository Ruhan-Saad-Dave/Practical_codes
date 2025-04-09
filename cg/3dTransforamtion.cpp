/*
Write C++ program to draw 3-D cube and perform following transformations on it using OpenGL
1) Scaling
2) Translation
3) Rotation about an axis (X/Y/Z)
*/

#include <GL/glut.h>
#include <GL/gl.h>
#include <iostream>

// Define rotation angles and scaling factors
float angleX = 0.0f, angleY = 0.0f, angleZ = 0.0f;
float scale = 1.0f;
float translateX = 0.0f, translateY = 0.0f, translateZ = -5.0f;

// Function to initialize OpenGL
void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Set the clear color to black
    glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D
}

// Function to draw the cube
void drawCube() {
    glBegin(GL_QUADS); // Begin drawing the cube

    // Front face
    glColor3f(1.0, 0.0, 0.0); // Red
    glVertex3f(-1.0, -1.0,  1.0);
    glVertex3f( 1.0, -1.0,  1.0);
    glVertex3f( 1.0,  1.0,  1.0);
    glVertex3f(-1.0,  1.0,  1.0);

    // Back face
    glColor3f(0.0, 1.0, 0.0); // Green
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0,  1.0, -1.0);
    glVertex3f( 1.0,  1.0, -1.0);
    glVertex3f( 1.0, -1.0, -1.0);

    // Left face
    glColor3f(0.0, 0.0, 1.0); // Blue
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f(-1.0, -1.0,  1.0);
    glVertex3f(-1.0,  1.0,  1.0);
    glVertex3f(-1.0,  1.0, -1.0);

    // Right face
    glColor3f(1.0, 1.0, 0.0); // Yellow
    glVertex3f(1.0, -1.0, -1.0);
    glVertex3f(1.0,  1.0, -1.0);
    glVertex3f(1.0,  1.0,  1.0);
    glVertex3f(1.0, -1.0,  1.0);

    // Top face
    glColor3f(1.0, 0.0, 1.0); // Magenta
    glVertex3f(-1.0, 1.0, -1.0);
    glVertex3f( 1.0, 1.0, -1.0);
    glVertex3f( 1.0, 1.0,  1.0);
    glVertex3f(-1.0, 1.0,  1.0);

    // Bottom face
    glColor3f(0.0, 1.0, 1.0); // Cyan
    glVertex3f(-1.0, -1.0, -1.0);
    glVertex3f( 1.0, -1.0, -1.0);
    glVertex3f( 1.0, -1.0,  1.0);
    glVertex3f(-1.0, -1.0,  1.0);

    glEnd(); // End drawing
}

// Function to perform transformations and draw the scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Apply transformations
    glTranslatef(translateX, translateY, translateZ); // Translation
    glScalef(scale, scale, scale); // Scaling
    glRotatef(angleX, 1.0, 0.0, 0.0); // Rotation around X
    glRotatef(angleY, 0.0, 1.0, 0.0); // Rotation around Y
    glRotatef(angleZ, 0.0, 0.0, 1.0); // Rotation around Z

    drawCube(); // Draw the cube

    glutSwapBuffers(); // Swap the front and back buffers
}

// Function to handle keyboard input for transformations
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': // Move forward
            translateZ += 0.1f;
            break;
        case 's': // Move backward
            translateZ -= 0.1f;
            break;
        case 'a': // Move left
            translateX -= 0.1f;
            break;
        case 'd': // Move right
            translateX += 0.1f;
            break;
        case 'q': // Rotate left
            angleY -= 5.0f;
            break;
        case 'e': // Rotate right
            angleY += 5.0f;
            break;
        case 'r': // Rotate up
            angleX += 5.0f;
            break;
        case 'f': // Rotate down
            angleX -= 5.0f;
            break;
        case 't': // Scale up
            scale += 0.1f;
            break;
        case 'g': // Scale down
            scale -= 0.1f;
            break;
        case 27: // Escape key
            exit(0);
            break;
    }
    glutPostRedisplay(); // Request to redraw the scene
}

// Main function
int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Cube Transformations");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop(); // Enter the GLUT event processing loop

    return 0;
}
