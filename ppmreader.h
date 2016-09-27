#pragma once
#include <cstdlib> 
#include <iostream>
#include <fstream>

#include "DEMarrays.h"
#include "ppm.h"
//#include <GL\glut.h>

using namespace std;
//Creates an object that takes in a filename or an array that returns a ppm object
class ppmreader
{
private:
		ppm * imageObject;
		unsigned char * memblock;
		char * memblock2;
		int height;
		int width;
		int colour;
		int firstDot;
		int secondDot;
		int firstSpace;
		int secondSpace;
		float *** heightMap;
		int pointer;
		unsigned char * imageMap;

public:
	ppmreader();
	~ppmreader();
	void setData();
	void setData2();
	void setupArray();
	void retrieveHeader();
	ppm * convertImage(char *fileName);
	ppm * getImage(char *fileName);
	ppm * getPanguImage(unsigned char * newArray);
	ppm * convertImagePANGU(unsigned char * newArray); 
	unsigned char * getPanguImage2(unsigned char * newArray);
	unsigned char * convertImagePANGU2(unsigned char * newArray);
};
