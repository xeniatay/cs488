// Code writen by: Vic Hollis 09/07/2003
// I don't mind if you use this class in your own code. All I ask is
// that you give me credit for it if you do.  And plug NeHe while your
// at it! :P
//////////////////////////////////////////////////////////////////////
// glVector.h: interface for the glVector class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLVECTOR_H__F526A5CF_89B5_4F20_8F2C_517D83879D35__INCLUDED_)
#define AFX_GLVECTOR_H__F526A5CF_89B5_4F20_8F2C_517D83879D35__INCLUDED_

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class glVector
{
public:
	void operator =(glVector v);
	glVector operator+(glVector v);
	glVector operator*(GLfloat scalar);
	void Normalize(void);
	GLfloat Magnitude(void);
	GLfloat m_Mag;
	void operator *=(GLfloat scalar);
	glVector();
	virtual ~glVector();

	GLfloat k;
	GLfloat j;
	GLfloat i;
};

#endif // !defined(AFX_GLVECTOR_H__F526A5CF_89B5_4F20_8F2C_517D83879D35__INCLUDED_)