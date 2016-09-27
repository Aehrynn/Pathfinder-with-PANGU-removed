#pragma once

class rover
{
	//A Rover object, contains information about the rover in a suitable form for processing
private:
	float height;
	float width;
	float depth;
	float x;
	float y;
	float z;
	float cameraZ;
	float modelX;
	float modelY;
	float heading;
	int range;
	
	
public:
	rover();
	rover(float newWidth, float newHeight, float newDepth, float newX, float newY, float newZ, float newCameraZ, int newRange);
	~rover();
	void setHeight(float newHeight);
	void setWidth(float newWidth);
	void setDepth(float newDepth);
	void setX(float newX);
	void setY(float newY);
	void setZ(float newZ);
	void setModelX(float newX);
	void setModelY(float newY);
	void setCameraZ(float newCameraZ);
	void setXY(float newX, float newY);
	void setHeading(float newX, float newY);
	void setRange(int newRange);
	int getRange();
	float getHeading();
	float getWidth();
	float getHeight();
	float getDepth();
	float getX();
	float getY();
	float getModelX();
	float getModelY();
	float getZ();
	float getCameraZ();
};