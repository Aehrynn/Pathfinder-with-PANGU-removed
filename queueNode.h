#pragma once
#include <stdio.h>

class queueNode
{
	//A queueNode that contains information on a coordinate
private:
	float x;
	float y;
	queueNode * next;

public:
	queueNode();
	~queueNode();
	queueNode(float newX, float newY);
	void setX(float newX);
	void setY(float newY);
	void setNext(queueNode * newNext);
	float getX();
	float getY();
	queueNode * getNext();
};