
#include "..\headers\DEMarrays.h"
#include <GL\glut.h>


int *singleArrayInt(int size)
{
	int *intArray = new int [size];
	return intArray;
}

float *singleArrayFloat(float size)
{
	float *floatArray = new float [size];
	return floatArray;
}

float **doubleArrayFloat(float height, float width)
{
	float **floatArray = new float *[height];
	for (int g = 0; g<height; g++)
		{
			floatArray[g] = new float [width];
		}
	return floatArray;
}

float ***tripleArrayFloat(float height, float width, float depth)
{
	float ***floatArray = new float **[height];
	for (int g = 0; g<height; g++)
		{
			floatArray[g] = new float *[width];
		}
	for (int y = 0; y<height; y++)
		{
			for (int k = 0; k<width; k++)
			{
				floatArray[y][k] = new float [depth];				
			}
		}
	return floatArray;
}

GLubyte *singleArrayGLubyte(float size)
{
	GLubyte *GLubyteArray = new GLubyte [size];
	return GLubyteArray;
}

GLubyte **doubleArrayGLubyte(float height, float width)
{
	GLubyte **GLubyteArray = new GLubyte *[height];
	for (int g = 0; g<height; g++)
		{
			GLubyteArray[g] = new GLubyte [width];
		}
	return GLubyteArray;
}

GLubyte ***tripleArrayGLubyte(float height, float width, float depth)
{
	GLubyte ***GLubyteArray = new GLubyte **[height];
	for (int g = 0; g<height; g++)
		{
			GLubyteArray[g] = new GLubyte *[width];
		}
	for (int y = 0; y<height; y++)
		{
			for (int k = 0; k<width; k++)
			{
				GLubyteArray[y][k] = new GLubyte [depth];				
			}
		}
	return GLubyteArray;
}

GLubyte *singleArrayGLubyte(float height, float width, float depth)
{
	GLubyte *GLubyteTest = new GLubyte [height*width*depth];
	return GLubyteTest;
}

void deleteArray(float height, float **deletedarray)
{
	//for a 2d array
	for (int i = 0; i < height; i++)
	{
		delete [] deletedarray[i];
	}
	delete [] deletedarray;
}

void deleteArray(float height, float width, float ***deletedarray)
{
	//for a 3d array
	for (int w = 0; w < width-1; w++)
	{
		for (int x = 0; x < height-1; x++)
		{
			delete [] deletedarray[w][x];
		}
		delete [] deletedarray[w];
	}
	delete [] deletedarray;
}