#include "queue.h"

queue::queue()
{
	current = NULL;
	last = NULL;
}

queue::~queue()
{
	//delete current;
	//delete last;
}

queueNode * queue::getCurrent()
{
	//return the current node at the front of the queue
	return current;
}

queueNode * queue::getLast()
{
	//return the current node at the front of the queue
	return last;
}
queueNode * queue::getNext()
{
	//Move up the queue by one
	if(current != NULL){
	current = current->getNext();
	return current;
	}else{
		//no queue
		//break;
		return current;
	}

}

void queue::addNode(float x, float y)
{
	pointer = new queueNode(x, y);
	
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