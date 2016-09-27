#pragma once
#include "queueNode.h"
#include <stdio.h>

class queue
{
	//A first-in first-out data structure
private:
	queueNode * current;
	queueNode * pointer;
	queueNode * last;

public:
	queue();
	~queue();
	queueNode * getCurrent();
	queueNode * getLast();
	queueNode * getNext();
	void addNode(float x, float y);
};
