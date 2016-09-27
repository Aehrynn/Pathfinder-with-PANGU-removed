#include "DEM.h"

DEM::DEM()
{
}

DEM::DEM(ppm * newImageObject)
{
	//imageObject = newImageObject;
	height = newImageObject->getHeight();
	width = newImageObject->getWidth();
	setImage(newImageObject);
	heightMap = doubleArrayFloat((float)height, (float)width);
	midpoint = 0;
	demImage = singleArrayGLubyte(height, width, 3);
	setHeightMap();
	makeDisplayImage();
}

DEM::~DEM()
{
	deleteArray(height, width, image);
	deleteArray(height, heightMap);
	delete [] demImage;
}

void DEM::setHeight(float newHeight)
{
	height = newHeight;
}


void DEM::setWidth(float newWidth)
{
	width = newWidth;
}


void DEM::setImage(ppm * newImageObject)
{
	//imageObject = newImageObject;
	image = tripleArrayFloat((float)height, (float)width, 3);
	for (int h = 0; h < height; h++)
	{
		for (int w = 0; w < width; w++)
		{
			for (int c = 0; c < 3; c++)
			{
				image[h][w][c]=newImageObject->getImage()[h][w][c];
			}
		}
	}
}
float DEM::getWidth()
{
	return width;
}

float DEM::getHeight()
{
	return height;
}
ppm * DEM::getImage()
{
	return imageObject;
}
void DEM::setHeightMap()
{
			//Sort through each 3 values to get the greyscale colour
		//256 * R + G and discard B
		
		for (int i = 0; i < height; i++)
		{
			for (int z =0; z < width; z++)
			{
				//Get Red value times by 256
				//heightMap[i][z] = float(((imageObject->getImage()[i][z][0])*256) + (imageObject->getImage()[i][z][1]));
				heightMap[i][z] = float(((image[i][z][0])*256) + (image[i][z][1]));	
				
				//pointer += 3;
			}
		}

		//Find the midpoint by finding the maximum and minimum from the dataset
		float maximum = 0;
		float minimum = 0;
		for (int t = 0; t < height; t++)
		{
			for (int y =0; y < width; y++)
			{
				if (heightMap[t][y] >= maximum){
				maximum = heightMap[t][y];
				}else{
					if (heightMap[t][y] <= minimum){
						minimum = heightMap[t][y];

					}
				}
				
			}
		}

		midpoint = (maximum - minimum)/2;
		
		//Now to revalue the values already in the array to regards of the midpoint being sealevel or ground level and taking into 
		//account the vertical scaling factor of the DEM which we will assume to be at 0.001
		float verticalScale = 0.001;
		for (int o = 0; o < height; o++)
		{
			for (int p =0; p < width; p++)
			{
				
				heightMap[o][p] = (heightMap[o][p] - midpoint) * verticalScale;
				
				
			}
		}
		//Creating an object with which to store the image attributes
		//imageObject = new ppm(float(width), float(height), float(255), heightMap);
}
float ** DEM::getHeightMap()
{
	return heightMap;
}
GLubyte * DEM::getDisplayImage()
{
	return demImage;
}
void DEM::makeDisplayImage()
{
	//Find the midpoint by finding the maximum and minimum from the dataset
		float max = 0;
		float min = 0;
		for (int t = 0; t < height; t++)
		{
			for (int y =0; y < width; y++)
			{
				if (heightMap[t][y] >= max){
				max = heightMap[t][y];
				}else{
					if (heightMap[t][y] <= min){
						min = heightMap[t][y];

					}
				}
				
			}
		}

		float range = max - min;

int arg = height-1;
int place = 0;
	for (int i = 0; i< height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				demImage[place] = (GLubyte) (((heightMap[arg][j]-min) * 256)/range);
				place++;
			}
		}
		arg--;
	}
	//Returns a 2DIM array
	//return demImage;
}


