#pragma once
#include "ppm.h"
#include "DEMarrays.h"
#include <GL\glut.h>

class DEM
{
	//A DEM(Digital Elevation Model) object, contains information about the DEM in a suitable form for processing
private:
	float height;
	float width;
	float midpoint;
	ppm * imageObject;
	float ** heightMap;
	float *** image;
	GLubyte * demImage;
public:
	DEM();
	DEM(ppm * newImageObject);
	~DEM();
	void setHeight(float newHeight);
	void setWidth(float newWidth);
	void setImage(ppm * newImageObject);
	float getWidth();
	float getHeight();
	ppm * getImage();
	void setHeightMap();
	float ** getHeightMap();
	GLubyte * getDisplayImage();
	void makeDisplayImage();
};