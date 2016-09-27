#pragma once
#include "DEM\headers\DEMarrays.h"
#include <GL\glut.h>


class ppm
{
	//An Image object, contains information about the ppm image in a suitable form for processing
private:
	float height;
	float width;
	float colour;
	float *** image;
	GLubyte * displayImage;
public:
	ppm();
	ppm(float newWidth, float newHeight, float newColour, float *** newImage);
	~ppm();
	void setHeight(float newHeight);
	void setWidth(float newWidth);
	void setColour(float newColour);
	void setImage(float *** newImage);
	float getWidth();
	float getHeight();
	float getColour();
	float *** getImage();
	void setupImage();
	GLubyte * getDisplayImage();
};