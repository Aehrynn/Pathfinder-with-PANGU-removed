#include "queueNode.h"

queueNode::queueNode()
{
	next = NULL;
}

queueNode::~queueNode()
{
	
}

queueNode::queueNode(float newX, float newY)
{
	setX(newX);
	setY(newY);
	next = NULL;
}

void queueNode::setX(float newX)
{
	x = newX;
}

void queueNode::setY(float newY)
{
	y = newY;
}

void queueNode::setNext(queueNode * newNext)
{
	next = newNext;
}

float queueNode::getX()
{
	return x;
}

float queueNode::getY()
{
	return y;
}

queueNode * queueNode::getNext()
{
	return next;
}