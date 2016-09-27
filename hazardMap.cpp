#include "hazardMap.h"

hazardMap::hazardMap()
{
	gradient = 0.35;
	gradient2 = 0.15;
	PATH = 50;
	ROVER = 90;
	hazMap = doubleArrayFloat((float)height, (float)width);
	
}

hazardMap::hazardMap(DEM * newDemObject)
{
	gradient = 0.35;
	gradient2 = 0.15;
	PATH = 50;
	//demObject = newDemObject;
	height = newDemObject->getHeight();
	width = newDemObject->getWidth();
	heightMap = newDemObject->getHeightMap();
	gradientMap = tripleArrayFloat((float)height, (float) width, (float) 9);
	hazMap = doubleArrayFloat((float)height, (float)width);
	hazImage3 = singleArrayInt((width*height));
	hazImage = singleArrayGLubyte(height, width, 3);
	calculateGradient();
	//makeDisplayImage();
}

hazardMap::hazardMap(float newWidth, float newHeight)
{
	gradient = 0.35;
	gradient2 = 0.15;
	PATH = 50;
	ROVER = 90;
	height = newHeight;
	width = newWidth;
	gradientMap = tripleArrayFloat((float)height, (float) width, (float) 9);
	hazMap = doubleArrayFloat((float)height, (float)width);
	hazImage3 = singleArrayInt((width*height));
	heightMap = doubleArrayFloat((float)height, (float)width);
	for(int x = 0; x <= height - 1; x++)
	{
		for(int y = 0; y <= width - 1; y++)
		{
			heightMap[x][y] = 0;
		}
	}
	hazImage = singleArrayGLubyte(height, width, 3);
	calculateGradient();
	//makeDisplayImage();
}

hazardMap::~hazardMap()
{
	deleteArray(height, width, gradientMap);
	deleteArray(height, hazMap);
	delete [] hazImage3; //1d array
	//delete [] heightMap;
	delete [] hazImage; //1d array
}

void hazardMap::setHeight(float newHeight)
{
	height = newHeight;
}


void hazardMap::setWidth(float newWidth)
{
	width = newWidth;
}

void hazardMap::setHeightMap(float ** newHeightMap)
{
	heightMap = newHeightMap;
}


float hazardMap::getWidth()
{
	return width;
}

float hazardMap::getHeight()
{
	return height;
}

float ** hazardMap::getHazardMap()
{
	return hazMap;
}

void hazardMap::setHazardMap(float ** newHazardMap)
{
	//hazMap = newHazardMap;
	for(int i = 0; i<height-1; i++)
	{
		for(int t =0; t<width-1; t++)
		{
			hazMap[i][t]=newHazardMap[i][t];
		}
	}
}

float ** hazardMap::getHeightMap()
{
	return heightMap;
}

int * hazardMap::getMap()
{
	return hazImage3;
}

//GLubyte * hazardMap::getDisplayImage()
//{
//	return hazImage;
//}
//void hazardMap::makeDisplayImage()
GLubyte * hazardMap::getDisplayImage()
{

	//From OpenGL book on making a checkerboard pattern
	int i, j;
	delete [] hazImage;
		hazImage = singleArrayGLubyte(height, width, 3);




arg = height-1;
place = 0;
	for (i = 0; i <= height-1; i++)
	{
		for ( j = 0; j <= width-1; j++)
		{
			
			if (hazMap[arg - i][j]==255)
			{
				//if the value of the pixel is set to 255, then its a hazard so colour red
				hazImage[place] = (GLubyte) (255);
				hazImage[place+1] = (GLubyte) (0);
				hazImage[place+2] = (GLubyte) (0);
				place = place + 3;
				//place++;
			}else if (hazMap[arg-i][j]==125){
				hazImage[place] = (GLubyte) (255);
				hazImage[place+1] = (GLubyte) (255);
				hazImage[place+2] = (GLubyte) (0);
				place = place + 3;
			}else if (hazMap[arg-i][j]==115){
				hazImage[place] = (GLubyte) (205);
				hazImage[place+1] = (GLubyte) (205);
				hazImage[place+2] = (GLubyte) (0);
				place = place + 3;
			}else if (hazMap[arg-i][j]==105){
				hazImage[place] = (GLubyte) (125);
				hazImage[place+1] = (GLubyte) (125);
				hazImage[place+2] = (GLubyte) (0);
				place = place + 3;
			}else if (hazMap[arg-i][j]==95){
				hazImage[place] = (GLubyte) (95);
				hazImage[place+1] = (GLubyte) (95);
				hazImage[place+2] = (GLubyte) (0);
				place = place + 3;
			}else if (hazMap[arg-i][j]==85){
				hazImage[place] = (GLubyte) (50);
				hazImage[place+1] = (GLubyte) (50);
				hazImage[place+2] = (GLubyte) (50);
				place = place + 3;
			}else if (hazMap[arg-i][j]==PATH){
				hazImage[place] = (GLubyte) (0);
				hazImage[place+1] = (GLubyte) (255);
				hazImage[place+2] = (GLubyte) (0);
				place = place + 3;
			}else if (hazMap[arg-i][j]==ROVER){
				hazImage[place] = (GLubyte) (0);
				hazImage[place+1] = (GLubyte) (0);
				hazImage[place+2] = (GLubyte) (125);
				place = place + 3;
			}else{
				//if the value of the pixel is not 255, so 0, then the terrain is 'walkable' so set to 0
				hazImage[place] = (GLubyte) (125);
				hazImage[place+1] = (GLubyte) (125);
				hazImage[place+2] = (GLubyte) (125);
				place = place + 3;
			}
		}
		//arg--;
	}

	
	return hazImage;
}
void hazardMap::calculateGradient()
{
			//gradientMap = tripleArrayFloat((float)height, (float) width, (float) 9);
			//hazMap = doubleArrayFloat((float)height, (float)width);
			//hazImage3 = singleArrayInt((width*height));
		//Average the values of the pixels - take the highest value from all the surrounding values
		float maxValue = 0;
		//int pixelPointer = 0;
		for (int rows = 0; rows < height-1; rows++)
		{
			for(int cols = 0; cols < width-1; cols++)
			{
				//For each pixel in range take the highest value as the one to use
				//maxValue = heightMap[rows][cols];
				
				
				for (int timesD = 0; timesD < 3; timesD++)
				{
					switch (timesD)
					{
							case 0: /*if (heightMap[rows][cols] > maxValue)
									{*/
										maxValue = heightMap[rows][cols];
										//cout << heightMap[rows][cols] << endl;
									//}
									break;
							case 1: if (heightMap[rows+1][cols] > maxValue)
										{
											maxValue = heightMap[rows+1][cols];
											//cout << heightMap[rows+1][cols] << endl;
										}
								   break;
							case 2: if (heightMap[rows+1][cols+1] > maxValue)
										{
											maxValue = heightMap[rows+1][cols+1];
											//cout << heightMap[rows+1][cols+1] << endl;
										}
									break;
							case 3: if (heightMap[rows][cols+1] > maxValue)
										{
											maxValue = heightMap[rows][cols+1];
											//cout << heightMap[rows][cols+1] << endl;
										}
									break;
					}
				//pixelPointer++;
				}
				//cout << maxValue << endl;
			heightMap[rows][cols]=maxValue;
			//cout << heightMap[rows][cols] << endl;
			//maxValue = 0;
			}
			
		}

	//Calculate the gradient of the heightmap
			//Calculate the gradient of each pixel, gradient = the change in y / the change in x, assuming here that the change in x will always
		//be one as its always the neighbouring pixel and that the horizontal scale of the DEM has been set to 1 like the vertical scale setting is 0.001
		//So for each pixel calculate it with all its neighbours, problem here will be the edges can't be taken as they won't have all the values available to them
		//So starting at [1][1] and finishing height-1 and width-1
		for (int rows = 1; rows < height - 1; rows++)
		{
			for(int cols = 1; cols < width - 1; cols++)
			{
				for(int depth = 0; depth < 9; depth++)
				{
					//start the calculations
					float changeY2 = 0.0;
					switch (depth){
						case 0:
							changeY2 = heightMap[rows - 1][cols - 1];
							break;
							case 1:
							changeY2 = heightMap[rows - 1][cols];
							break;
							case 2:
							changeY2 = heightMap[rows - 1][cols + 1];
							break;
							case 3:
							changeY2 = heightMap[rows][cols - 1];
							break;
							case 4:
							changeY2 = heightMap[rows][cols];
							break;
							case 5:
							changeY2 = heightMap[rows][cols + 1];
							break;
							case 6:
							changeY2 = heightMap[rows + 1][cols - 1];
							break;
							case 7:
							changeY2 = heightMap[rows + 1][cols];
							break;
							case 8:
							changeY2 = heightMap[rows + 1][cols + 1];
							break;
						default:
							break;

					}

					gradientMap[rows][cols][depth] = changeY2 - heightMap[rows][cols];
					
				}
			}
		}
	//cout << "The gradient in the last pixel to its south-east "<<gradientMap[5][5][3] << endl;
	for (int widthHeight = 0; widthHeight < (width*height); widthHeight ++)
	{
		hazImage3[widthHeight] = 0;
	}
		//Now that the gradient of the surrounding neighbours have been taken, for each pixel if the gradient 
		//is higher than the setting then it is a hazard, 255, otherwise set to 0 and is 'walkable'
	
	for (int rows = 1; rows < height - 1; rows++)
		{
			for(int cols = 1; cols < width - 1; cols++)
			{
				for(int depth = 0; depth < 9; depth++)
				{
					//Once neighbour has been found to have a gradient more than the setting
					//then break out of this loop and go onto the next pixel
					if (hazMap[rows][cols]!=255){
					if (gradientMap[rows][cols][depth]>=gradient)
					{
						hazMap[rows][cols]=255;
						//hazImage3[(cols*width)+rows]=9;
					}else if(gradientMap[rows][cols][depth]>=gradient2)
					{
						hazMap[rows][cols]=125;
					}else if(gradientMap[rows][cols][depth]>=(float)0.09)
					{
						hazMap[rows][cols]=115;
					}else if(gradientMap[rows][cols][depth]>=(float)0.07)
					{
						hazMap[rows][cols]=105;
					}else if(gradientMap[rows][cols][depth]>=(float)0.05)
					{
						hazMap[rows][cols]=95;
					}else if(gradientMap[rows][cols][depth]>=(float)0.00)
					{
						hazMap[rows][cols]=85;
					}else{
						hazMap[rows][cols]=0;
					}
					}else{
						//do nothing
					}
				}
			}
	}
	int holder;
for (int rows = 1; rows < height - 1; rows++)
		{
			for(int cols = 1; cols < width - 1; cols++)
			{
				if(hazMap[rows][cols]==255)
				{
					holder = (cols*width)+rows;
					hazImage3[holder]=9;
				}else if(hazMap[rows][cols]==125)
				{
					holder = (cols*width)+rows;
					hazImage3[holder]=5;
				}else if(hazMap[rows][cols]==115)
				{
					holder = (cols*width)+rows;
					hazImage3[holder]=4;
				}else if(hazMap[rows][cols]==105)
				{
					holder = (cols*width)+rows;
					hazImage3[holder]=3;
				}else if(hazMap[rows][cols]==95)
				{
					holder = (cols*width)+rows;
					hazImage3[holder]=2;
				}else if(hazMap[rows][cols]==85)
				{
					holder = (cols*width)+rows;
					hazImage3[holder]=1;
				}
			}
		}

}

void hazardMap::updateHazards()
{
	//Takes in an Array, xy position and a heading to add in new data to the hazardMap
	//x and y is the rover position - newHeading tells us where we are looking at
		int holder;
for (int rows = 1; rows < height - 1; rows++)
		{
			for(int cols = 1; cols < width - 1; cols++)
			{
				if(hazMap[rows][cols]==255)
				{
					holder = (cols*width)+rows;
					hazImage3[holder]=9;
				}else if(hazMap[rows][cols]==125)
				{
					holder = (cols*width)+rows;
					hazImage3[holder]=5;
				}else if(hazMap[rows][cols]==115)
				{
					holder = (cols*width)+rows;
					hazImage3[holder]=4;
				}else if(hazMap[rows][cols]==105)
				{
					holder = (cols*width)+rows;
					hazImage3[holder]=3;
				}else if(hazMap[rows][cols]==95)
				{
					holder = (cols*width)+rows;
					hazImage3[holder]=2;
				}else if(hazMap[rows][cols]==85)
				{
					holder = (cols*width)+rows;
					hazImage3[holder]=1;
				}
			}
		}
}
