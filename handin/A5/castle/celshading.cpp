/*
 * This code was created by Jeff Molofee '99
 *(merged to Linux/SDL by Sean Farrel form Ti Leggett's
 * Linux/SDL Basecode and Kah-Wah Tang port to Linux/GLUT
 * of this Tutorial)
 *
 *
 * If you've found this code useful, please let me know.
 *
 * Visit Jeff at http://nehe.gamedev.net/
 *
 * or for port-specific comments, questions, bugreports etc.
 * email to leggett@eecs.tulane.edu
 */

#include "celshading.hpp"

// User Defined Variables
bool  outlineDraw = true;               // Flag To Draw The Outline
bool  outlineSmooth = false;              // Flag To Anti-Alias The Lines
float outlineColor[3] = { 0.0f, 0.0f, 0.0f };       // Color Of The Lines
float outlineWidth  = 3.0f;               // Width Of The Lines

VECTOR  lightAngle;                     // The Direction Of The Light
bool  lightRotate = false;              // Flag To See If We Rotate The Light

float modelAngle  = 0.0f;               // Y-Axis Angle Of The Model
bool    modelRotate = true;             // Flag To Rotate The Model

POLYGON *polyData = NULL;               // Polygon Data
int polyNum   = 0;                // Number Of Polygons

GLuint  shaderTexture[1];                 // Storage For One Texture

bool ReadMesh(void) // Reads The Contents Of The "model.txt" File
{
  FILE *In = fopen("./Model.txt", "rb");          // Open The File

  if(!In)
    return false;                     // Return FALSE If File Not Opened

  fread(&polyNum, sizeof(int), 1, In);            // Read The Header(i.e. Number Of Polygons)

  polyData = new POLYGON [polyNum];             // Allocate The Memory

  fread(&polyData[0], sizeof(POLYGON) * polyNum, 1, In);  // Read In All Polygon Data

  fclose(In);                        // Close The File

  return true;                        // It Worked
}

// Math Functions
inline float DotProduct(VECTOR &V1, VECTOR &V2)        // Calculate The Angle Between The 2 Vectors
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;       // Return The Angle
}

inline float Magnitude(VECTOR &V)                // Calculate The Length Of The Vector
{
  return sqrtf(V.X * V.X + V.Y * V.Y + V.Z * V.Z);     // Return The Length Of The Vector
}

void Normalize(VECTOR &V)                    // Creates A Vector With A Unit Length Of 1
{
  float M = Magnitude(V);                  // Calculate The Length Of The Vector

  if(M != 0.0f)                        // Make Sure We Don't Divide By 0
  {
    V.X /= M;                       // Normalize The 3 Components
    V.Y /= M;
    V.Z /= M;
  }
}

void RotateVector(MATRIX &M, VECTOR &V, VECTOR &D)       // Rotate A Vector Using The Supplied Matrix
{
  D.X =(M.Data[0] * V.X) +(M.Data[4] * V.Y) +(M.Data[8]  * V.Z); // Rotate Around The X Axis
  D.Y =(M.Data[1] * V.X) +(M.Data[5] * V.Y) +(M.Data[9]  * V.Z); // Rotate Around The Y Axis
  D.Z =(M.Data[2] * V.X) +(M.Data[6] * V.Y) +(M.Data[10] * V.Z); // Rotate Around The Z Axis
}

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
bool initGL()         // We call this right after our OpenGL window is created.
{

  int i;                            // Looping Variable
  char Line[255];                       // Storage For 255 Characters
  double shaderData[32][3];                  // Storate For The 96 Shader Values

  FILE *In  = NULL;                     // File Pointer

  //g_window  = window;
  //g_keys  = keys;

  // Start Of User Initialization
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);     // Realy Nice perspective calculations

  glClearColor(0.7f, 0.7f, 0.7f, 0.0f);            // Light Grey Background
  glClearDepth(1.0f);                    // Depth Buffer Setup

  glEnable(GL_DEPTH_TEST);                 // Enable Depth Testing
  glDepthFunc(GL_LESS);                    // The Type Of Depth Test To Do

  glShadeModel(GL_SMOOTH);                 // Enables Smooth Color Shading
  glDisable(GL_LINE_SMOOTH);                 // Initially Disable Line Smoothing

  glEnable(GL_CULL_FACE);                  // Enable OpenGL Face Culling

  glDisable(GL_LIGHTING);                  // Disable OpenGL Lighting

  In = fopen("./Shader.txt", "r");            // Open The Shader File

  if(In)                           // Check To See If The File Opened
  {
    for(i = 0; i < 32; i++)                // Loop Though The 32 Greyscale Values
    {
      if(feof(In))                    // Check For The End Of The File
        break;

      fgets(Line, 255, In);                // Get The Current Line

      shaderData[i][0] = shaderData[i][1] = shaderData[i][2] = double(atof(Line)); // Copy Over The Value
    }

    fclose(In);                      // Close The File
  }

  else
    return false;                     // It Went Horribly Horribly Wrong

  glGenTextures(1, &shaderTexture[0]);           // Get A Free Texture ID

  glBindTexture(GL_TEXTURE_1D, shaderTexture[0]);      // Bind This Texture. From Now On It Will Be 1D

  // For Crying Out Loud Don't Let OpenGL Use Bi/Trilinear Filtering!
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, 32, 0, GL_RGB , GL_FLOAT, shaderData);  // Upload

  lightAngle.X = 0.0f;                    // Set The X Direction
  lightAngle.Y = 0.0f;                    // Set The Y Direction
  lightAngle.Z = 1.0f;                    // Set The Z Direction

  Normalize(lightAngle);
                    // Normalize The Light Direction
  return ReadMesh();
}

/* The main drawing function. */
void DrawGLScene() {
  initGL();

  int i, j;                         // Looping Variables
  float TmpShade;                       // Temporary Shader Value
  MATRIX TmpMatrix;                     // Temporary MATRIX Structure
  VECTOR TmpVector, TmpNormal;                // Temporary VECTOR Structures

  // Check To See If We Want Anti-Aliased Lines
  if(outlineSmooth) {
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);        // Use The Good Calculations
    glEnable(GL_LINE_SMOOTH);                // Enable Anti-Aliasing
  } else {
    glDisable(GL_LINE_SMOOTH);               // Disable Anti-Aliasing
  }

  glGetFloatv(GL_MODELVIEW_MATRIX, TmpMatrix.Data);      // Get The Generated Matrix

  // Cel-Shading Code //
  glEnable(GL_TEXTURE_1D);                 // Enable 1D Texturing
  glBindTexture(GL_TEXTURE_1D, shaderTexture[0]);      // Bind Our Texture
  glColor3f(1.0f, 1.0f, 1.0f);               // Set The Color Of The Model

  glBegin(GL_TRIANGLES);
    for(i = 0; i < polyNum; i++) {
      for(j = 0; j < 3; j++) {
        TmpNormal.X = polyData[i].Verts[j].Nor.X;   // Fill Up The TmpNormal Structure With
        TmpNormal.Y = polyData[i].Verts[j].Nor.Y;   // The Current Vertices' Normal Values
        TmpNormal.Z = polyData[i].Verts[j].Nor.Z;

        RotateVector(TmpMatrix, TmpNormal, TmpVector); // Rotate This By The Matrix
        Normalize(TmpVector);              // Normalize The New Normal
        TmpShade = DotProduct(TmpVector, lightAngle);  // Calculate The Shade Value
        if(TmpShade < 0.0f) {
          TmpShade = 0.0f;              // Clamp The Value to 0 If Negative
        }
        glTexCoord1f(TmpShade);            // Set The Texture Co-ordinate As The Shade Value
        glVertex3fv(&polyData[i].Verts[j].Pos.X);    // Send The Vertex Position
      }
    }
  glEnd();
  glDisable(GL_TEXTURE_1D);                  // Disable 1D Textures

  // Outline Code //
  if(outlineDraw) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    // Draw Backfacing Polygons As Wireframes
    glPolygonMode(GL_BACK, GL_LINE);
    // Set The Line Width
    glLineWidth(outlineWidth);
    glCullFace(GL_FRONT);                  // Don't Draw Any Front-Facing Polygons
    glDepthFunc(GL_LEQUAL);                // Change The Depth Mode
    glColor3fv(&outlineColor[0]);              // Set The Outline Color

    glBegin(GL_TRIANGLES);
      for(i = 0; i < polyNum; i++) {
        for(j = 0; j < 3; j++) {
          glVertex3fv(&polyData[i].Verts[j].Pos.X);
        }
      }
    glEnd();

    glDepthFunc(GL_LESS); // Reset The Depth-Testing Mode
    glCullFace(GL_BACK); // Reset The Face To Be Culled
    glPolygonMode(GL_BACK, GL_FILL); // Reset Back-Facing Polygon Drawing Mode
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
  }

/* Rotation stuff
  //glTranslatef(0.0f, 0.0f, -2.0f);             // Move 2 Units Away From The Screen
  //glRotatef(modelAngle, 0.0f, 1.0f, 0.0f);         // Rotate The Model On It's Y-Axis
  if(modelRotate) {
    modelAngle += 2.0f;
  }
*/
}

void Deinitialize(void)                    // Any User DeInitialization Goes Here
{
  glDeleteTextures(1, &shaderTexture[0]);          // Delete The Shader Texture

  delete [] polyData;                     // Delete The Polygon Data
}

