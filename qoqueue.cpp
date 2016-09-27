#include "qoqueue.h"

qoqueue::qoqueue()
{
	current = NULL;
	last = NULL;
}

qoqueue::~qoqueue()
{
	delete [] current;
	delete [] last;
}

qqnode * qoqueue::getCurrent()
{
	//return the current node at the front of the queue
	return current;
}

qqnode * qoqueue::getLast()
{
	//return the current node at the front of the queue
	return last;
}
qqnode * qoqueue::getNext()
{
	//Move up the queue by one
	if(current != NULL){
		pointer = current;
	current = current->getNext();
	delete pointer;
	return current;
	}else{
		//no queue
		//break;
		return current;
	}

}

void qoqueue::addNode(queue * newData)
{
	pointer = new qqnode(newData);
	
	if (last == NULL)
	{
		//then queue is empty
		current = pointer;
		last = pointer;
	}else{
		//queue not empty so set the next of the last to the newNode then set newNode to be the new last node
		last->setNext(pointer);
		last = pointer;
	}

	
}