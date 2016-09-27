#pragma once
#include "queueNode.h"
#include "queue.h"

class qqnode
{
private:
	queue * data;
	qqnode * next;

public:
	qqnode();
	~qqnode();
	qqnode(queue * newData);
	void setData(queue * newData);
	void setNext(qqnode * newNext);
	queue * getData();
	qqnode * getNext();
};