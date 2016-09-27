#pragma once
#include "queue.h"
#include "queueNode.h"
#include "astar.h"
#include "hazardMap.h"
#include "qoqueue.h"
#include <stdio.h>

class pathList
{
	//Make a queue that holds a queue for a set of points
	//This handles multiple waypoints for the pathfinding algorithm
private:
	queue * totalQueue;

	astar * roverPath;
	hazardMap * hazMap;
	qoqueue * thePath;
	queue * waypoints;
	bool pathstatus;

public:
	pathList();
	~pathList();
	pathList(queue * newQueue, hazardMap * newhazMap);	
	void createPath();
	qoqueue * theWay();
	bool getStatus();

};