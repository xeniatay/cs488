//
// This code was created by Jeff Molofee '99 (ported to Linux/GLUT by Kah-Wah Tang 2002 with help from the lesson 1 basecode for Linux by Richard Campbell)
//
// If you've found this code useful, please let me know.
//
// Visit me at www.xs4all.nl/~chtang
// (email Kah-Wah Tang at tombraider28@hotmail.com)
//
#ifndef CS488_CELSHADING_HPP
#define CS488_CELSHADING_HPP

#include <GL/glut.h>    // Header File For The GLUT Library
#include <GL/gl.h>  // Header File For The OpenGL32 Library
#include <GL/glu.h> // Header File For The GLu32 Library
#include <unistd.h>     // Header file for sleeping.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* ascii code for the escape key */
#define ESCAPE 27
#define FALSE 0
#define TRUE 1

/* The number of our GLUT window */
extern int window;

#ifndef CDS_FULLSCREEN                      // CDS_FULLSCREEN Is Not Defined By Some
#define CDS_FULLSCREEN 4                    // Compilers. By Defining It This Way,
#endif                              // We Can Avoid Errors

// User Defined Structures
typedef struct tagMATRIX                    // A Structure To Hold An OpenGL Matrix ( NEW )
{
  float Data[16];                       // We Use [16] Due To OpenGL's Matrix Format ( NEW )
} MATRIX;

typedef struct tagVECTOR                    // A Structure To Hold A Single Vector ( NEW )
{
  float X, Y, Z;                        // The Components Of The Vector ( NEW )
} VECTOR;

typedef struct tagVERTEX                    // A Structure To Hold A Single Vertex ( NEW )
{
  VECTOR Nor;                         // Vertex Normal ( NEW )
  VECTOR Pos;                         // Vertex Position ( NEW )
} VERTEX;

typedef struct tagPOLYGON                   // A Structure To Hold A Single Polygon ( NEW )
{
  VERTEX Verts[3];                      // Array Of 3 VERTEX Structures ( NEW )
} POLYGON;

// User Defined Variables
extern bool  outlineDraw;               // Flag To Draw The Outline ( NEW )
extern bool  outlineSmooth;              // Flag To Anti-Alias The Lines ( NEW )
extern float outlineColor[3];       // Color Of The Lines ( NEW )
extern float outlineWidth;               // Width Of The Lines ( NEW )

extern VECTOR lightAngle;                     // The Direction Of The Light ( NEW )
extern bool lightRotate;              // Flag To See If We Rotate The Light ( NEW )

extern float modelAngle;               // Y-Axis Angle Of The Model ( NEW )
extern bool modelRotate;             // Flag To Rotate The Model ( NEW )

extern POLYGON *polyData;               // Polygon Data ( NEW )
extern int polyNum;                // Number Of Polygons ( NEW )

extern GLuint  shaderTexture[1];                 // Storage For One Texture ( NEW )

// File Functions

// Reads The Contents Of The "model.txt" File ( NEW )
bool ReadMesh(void);

// Math Functions

// Calculate The Angle Between The 2 Vectors ( NEW )
inline float DotProduct (VECTOR &V1, VECTOR &V2);

// Calculate The Length Of The Vector ( NEW )
inline float Magnitude (VECTOR &V);

// Creates A Vector With A Unit Length Of 1 ( NEW )
void Normalize (VECTOR &V);

// Rotate A Vector Using The Supplied Matrix ( NEW )
void RotateVector (MATRIX &M, VECTOR &V, VECTOR &D);

/* A general OpenGL initialization function.  Sets all of the initial parameters. */

// We call this right after our OpenGL window is created.
bool InitGL(int Width, int Height);

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void ReSizeGLScene(int Width, int Height);

/* The main drawing function. */
void DrawGLScene();

// Any User DeInitialization Goes Here
void Deinitialize (void);

/* The function called whenever a key is pressed. */
void keyPressed(unsigned char key, int x, int y);
void specialKeyPressed(int key, int x, int y);

/*
int main(int argc, char **argv)
{
  // Initialize GLUT state - glut will take any command line arguments that pertain to it or
  //   X Windows - look at its documentation at http://reality.sgi.com/mjk/spec3/spec3.html *
  glutInit(&argc, argv);

  // Select type of Display mode:
  //   Double buffer
  //   RGBA color
  //   Alpha components supported
  //   Depth buffer *
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

  // get a 640 x 480 window *
  glutInitWindowSize(640, 480);

  // the window starts at the upper left corner of the screen *
  glutInitWindowPosition(0, 0);

  // Open a window *
  window = glutCreateWindow("Jeff Molofee's GL Code Tutorial ... NeHe '99");

  // Register the function to do all our OpenGL drawing. *
  glutDisplayFunc(&DrawGLScene);

  // Go fullscreen.  This is as soon as possible. *
  glutFullScreen();

  // Even if there are no events, redraw our gl scene.*
  glutIdleFunc(&DrawGLScene);

  // Register the function called when our window is resized.
  glutReshapeFunc(&ReSizeGLScene);

  // Register the function called when the keyboard is pressed.
  glutKeyboardFunc(&keyPressed);

  glutSpecialFunc(&specialKeyPressed);

  // Initialize our window.
  InitGL(640, 480);

  // Start Event Processing Engine
  glutMainLoop();

  return 1;
}
*/

#endif
