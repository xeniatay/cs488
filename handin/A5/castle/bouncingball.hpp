//http://www3.ntu.edu.sg/home/ehchua/programming/opengl/CG_Introduction.html
#ifndef CS488_BOUNCINGBALL_HPP
#define CS488_BOUNCINGBALL_HPP
/*
 * GL07BouncingBall.cpp: A ball bouncing inside the window
 */
#include <GL/glut.h>  // GLUT, includes glu.h and gl.h
#include <math.h>     // Needed for sin, cos
#include <iostream>

using namespace std;

#define PI 3.14159265f

// Global variables
extern int windowWidth;
extern int windowHeight;
extern int windowPosX;
extern int windowPosY;

extern GLfloat ballRadius;
extern GLfloat ballX;
extern GLfloat ballY;
extern GLfloat ballXMax, ballXMin, ballYMax, ballYMin; // Ball's center (x, y) bounds
extern GLfloat xSpeed;
extern GLfloat ySpeed;
extern int refreshMillis;

// Projection clipping area
extern GLdouble clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop;

/* Callback handler for window re-paint event */
void display_bball();
void reshape_bball(GLsizei width, GLsizei height);
/* Main function: GLUT runs as a console application starting at main() */
/*
int main(int argc, char** argv) {
   glutInit(&argc, argv);            // Initialize GLUT
   glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
   glutInitWindowSize(windowWidth, windowHeight);  // Initial window width and height
   glutInitWindowPosition(windowPosX, windowPosY); // Initial window top-left corner (x, y)
   glutCreateWindow(title);      // Create window with given title
   glutDisplayFunc(display_bball);     // Register callback handler for window re-paint
   glutReshapeFunc(reshape);     // Register callback handler for window re-shape
   glutTimerFunc(0, Timer, 0);   // First timer call immediately
   //initGL();                     // Our own OpenGL initialization
   glutMainLoop();               // Enter event-processing loop
   return 0;
}
*/
#endif