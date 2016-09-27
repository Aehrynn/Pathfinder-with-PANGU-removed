#include "ppm.h"
//Code List for PPM Image Object
ppm::ppm()
{
	setWidth(0);
	setHeight(0);
	setColour(0);
	setImage(0);
	displayImage = singleArrayGLubyte(height, width, 3);
}

ppm::ppm(float newWidth, float newHeight, float newColour, float *** newImage)
{
	setWidth(newWidth);
	setHeight(newHeight);
	setColour(newColour);
	setImage(newImage);
	displayImage = singleArrayGLubyte(height, width, 3);

}

ppm::~ppm()
{
	delete [] displayImage;
	deleteArray(height, width, image);
}

void ppm::setColour(float newColour)
{
	colour = newColour;
}

void ppm::setHeight(float newHeight)
{
	height = newHeight;
}

void ppm::setupImage()
{
	displayImage = singleArrayGLubyte(height, width, 3);
}

void ppm::setWidth(float newWidth)
{
	width = newWidth;
}

void ppm::setImage(float *** newImage)
{
	//image = newImage;
	image = tripleArrayFloat((float)height, (float)width, 3);
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			for (int c = 0; c < 3; c++)
			{
				image[h][w][c]=newImage[h][w][c];
			}
		}
	}
}

float ppm::getHeight()
{
	return height;
}

float ppm::getColour()
{
	return colour;
}
float ppm::getWidth()
{
	return width;
}
float *** ppm::getImage()
{
	return image;
}

GLubyte * ppm::getDisplayImage()
{
	//Convert 3 dimensional array to a 1 dimensional array for display purposes
	delete [] displayImage;
	displayImage = singleArrayGLubyte(height, width, 3);
	int arg = height-1;
int place = 0;
	for (int i = 0; i< height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			
				displayImage[place] = (GLubyte)image[arg][j][0];
				displayImage[place+1] =(GLubyte)image[arg][j][1];
				displayImage[place+2] = (GLubyte)image[arg][j][2];
				place += 3;
			
		}
		arg--;
	}
	return displayImage;
}