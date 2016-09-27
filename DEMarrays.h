#pragma once
#include <GL\glut.h>

	int *singleArrayInt(int);
	float *singleArrayFloat(float);
	float **doubleArrayFloat(float, float);
	float ***tripleArrayFloat(float, float, float);
	GLubyte *singleArrayGLubyte(float);
	GLubyte **doubleArrayGLubyte(float, float);
	GLubyte ***tripleArrayGLubyte(float, float, float);
	GLubyte *singleArrayGLubyte(float, float, float);
	void deleteArray(float, float**);
	void deleteArray(float, float, float***);

