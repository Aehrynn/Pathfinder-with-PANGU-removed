#pragma once
#include "qqnode.h"
#include "queue.h"
#include <stdio.h>

class qoqueue
{
private:
	qqnode * current;
	qqnode * pointer;
	qqnode * last;

public:
	qoqueue();
	~qoqueue();
	qqnode * getCurrent();
	qqnode * getLast();
	qqnode * getNext();
	void addNode(queue * newData);
};