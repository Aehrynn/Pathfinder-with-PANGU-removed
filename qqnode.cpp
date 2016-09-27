#include "qqnode.h"

qqnode::qqnode()
{
	next = NULL;
	data = NULL;
}

qqnode::~qqnode()
{
	//delete [] data;
}
qqnode::qqnode(queue * newData)
{
	data = newData;
	next = NULL;
}

void qqnode::setData(queue * newData)
{
	data = newData;
}

void qqnode::setNext(qqnode * newNext)
{
	next = newNext;
}

queue * qqnode::getData()
{
	return data;
}

qqnode * qqnode::getNext()
{
	return next;
}