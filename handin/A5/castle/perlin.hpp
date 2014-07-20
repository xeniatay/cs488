#ifndef CS488_PERLIN_HPP
#define CS488_PERLIN_HPP

#include <stdlib.h>
#include <math.h>
#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include "algebra.hpp"

using namespace std;

#define MAXB 0x100
#define N 0x1000
#define NP 12   // 2^N
#define NM 0xfff

#define s_curve(t) ( t * t * (3. - 2. * t) )
#define lerp(t, a, b) ( a + t * (b - a) )
#define setup(i, b0, b1, r0, r1)\
        t = vec[i] + N;\
        b0 = ((int)t) & BM;\
        b1 = (b0+1) & BM;\
        r0 = t - (int)t;\
        r1 = r0 - 1.;
#define at2(rx, ry) ( rx * q[0] + ry * q[1] )
#define at3(rx, ry, rz) ( rx * q[0] + ry * q[1] + rz * q[2] )


extern int Noise3DTexSize;
extern GLubyte* Noise3DTexPtr;

extern int p[MAXB + MAXB + 2];
extern double g3[MAXB + MAXB + 2][3];
extern double g2[MAXB + MAXB + 2][2];
extern double g1[MAXB + MAXB + 2];

extern int start;
extern int B;
extern int BM;

void init3DNoiseTexture();
void make3DNoiseTexture();
void initNoise();
void CreateNoise3D();
void SetNoiseFrequency(int frequency);
double noise1(double arg);
double noise2(double vec[2]);
double noise3(double vec[3]);
void normalize2(double v[2]);
void normalize3(double v[3]);
void initNoise();
double PerlinNoise1D(double x,double alpha,double beta,int n);
double PerlinNoise2D(double x, double y, double alpha, double beta, int n);
double PerlinNoise3D(double x, double y, double z, double alpha, double beta, int n);
void make3DNoiseTexture(double startFrequency, double amp);
void makeSkyTexture(double startFrequency, double amp);
void init3DNoiseTexture();
double cloudExpCurve(double v);
Colour getColour(Colour &start, Colour &end, double u);

#endif