#include "rover.h"


	rover::rover()
	{
		range = 0;
		 height = 0;
	 width = 0;
	 depth = 0;
	 x = 0;
	y = 0;
	z = 0;
	 cameraZ = z + 1;
	 heading = 0;
	 setModelX(x);
	 setModelY(y);

	}

	rover::rover(float newWidth, float newHeight, float newDepth, float newX, float newY, float newZ, float newCameraZ, int newRange)
	{
		setHeight(newHeight);
		setWidth(newWidth);
		setDepth(newDepth);
		setX(newX);
		setY(newY);
		setZ(newZ);
		setCameraZ(newCameraZ);
		setModelX(newX);
		setModelY(newY);
		setRange(newRange);

	}

	rover::~rover()
	{
	}

	void rover::setHeight(float newHeight)
	{
		height = newHeight;
	}

	void rover::setWidth(float newWidth)
	{
		width = newWidth;
	}

	void rover::setDepth(float newDepth)
	{
		depth = newDepth;
	}

	void rover::setX(float newX)
	{
		x = newX;
		setModelX(newX);
	}

	void rover::setY(float newY)
	{
		y = newY;
		setModelY(newY);
	}

	void rover::setXY(float newX, float newY)
	{
		setHeading(newX, newY);
		/*x = newX;
		y = newY;
		setModelY(newY);
		setModelX(newX);*/
	}

	void rover::setZ(float newZ)
	{
		z = newZ;
	}

	void rover::setCameraZ(float newCameraZ)
	{
		cameraZ = newCameraZ;
	}

	float rover::getWidth()
	{
		return width;
	}

	float rover::getHeight()
	{
		return height;
	}

	float rover::getDepth()
	{
		return depth;
	}

	float rover::getX()
	{
		return x;
	}

	float rover::getY()
	{
		return y;
	}

	float rover::getZ()
	{
		return z;
	}

	float rover::getCameraZ()
	{
		return cameraZ;
	}

	void rover::setModelX(float newX)
	{
		//Convert map coordinates to model coordinates
		//Map starts at top left - model in centre
		
			modelX = newX - (width/2);
		
	}

	void rover::setModelY(float newY)
	{
		//Convert map coordinates to model coordinates
		//Map starts at top left - model in centre
		
			modelY = -1*(newY - (height/2));
		
	}

	float rover::getModelX()
	{
		return modelX;
	}

	float rover::getModelY()
	{
		return modelY;
	}

	void rover::setHeading(float newX, float newY)
	{
		float NORTH = 0;
		float NORTHEAST = 45;
		float EAST = 90;
		float SOUTHEAST = 135;
		float SOUTH = 180;
		float SOUTHWEST = 225;
		float WEST = 270;
		float NORTHWEST = 315;
		/*float NORTH = 90;
		float NORTHEAST = 45;
		float EAST = 0;
		float SOUTHEAST = 315;
		float SOUTH = 270;
		float SOUTHWEST = 225;
		float WEST = 180;
		float NORTHWEST = 135;*/
		//sets the direction the rover is looking at
		if (newY > y)
		{
			//south
			if (newX > x)
			{
				//south-east
				heading = SOUTHEAST;
			}else if(newX < x)
			{
				//south-west
				heading = SOUTHWEST;
			}else
			{
				//just south
				heading = SOUTH;
			}

		}else if(newY < y)
		{
			//north
			if (newX > x)
			{
				//north-east
				heading = NORTHEAST;
			}else if(newX < x)
			{
				//north-west
				heading = NORTHWEST;
			}else
			{
				//just north
				heading = NORTH;
			}
		}else if (newX > x)
		{
			//east
			heading = EAST;
		}else if (newX < x)
		{
			//west
			heading = WEST;
		}

		x = newX;
		y = newY;
		setModelY(newY);
		setModelX(newX);

	}

	float rover::getHeading()
	{
		return heading;
	}

	void rover::setRange(int newRange)
	{
		range = newRange;
	}

	int rover::getRange()
	{
		return range;
	}