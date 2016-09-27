#pragma once
#include <iostream>
#include "ppm.h"
#include "DEMarrays.h"
#include <GL\glut.h>
#include "DEM.h"

using namespace std;

class hazardMap
{
		//An Image object, contains information about the ppm image in a suitable form for processing
private:
	float height;
	float width;
	//DEM * demObject;
	float ** heightMap;
	float ***gradientMap;
	float ** hazMap;
	GLubyte * hazImage;
	int * hazImage3;
	float gradient;
	float gradient2;
	float PATH;
	float ROVER;
	int arg;
int place;

public:
	hazardMap();
	hazardMap(DEM * newDemObject);
	hazardMap(float newWidth, float newHeight);
	~hazardMap();
	void setHeight(float newHeight);
	void setWidth(float newWidth);
	float getWidth();
	float getHeight();
	float ** getHazardMap();
	void setHeightMap(float ** newHeightMap);
	void setHazardMap(float ** newHazardMap);
	float ** getHeightMap();
	GLubyte * getDisplayImage();
	void makeDisplayImage();
	void calculateGradient();
	int * getMap();
	void updateHazards();
};


