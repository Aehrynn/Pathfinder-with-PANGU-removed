#pragma once
#include "stlastar.h"
#include "hazardMap.h"
#include "queue.h"


class astar
{
private:
	float PATH;
	hazardMap * hazMap;
	int startX;
	int startY;
	int endX;
	int endY;
	queue * queueList;
	bool STATUS;

public:
	astar();
	astar(int newX, int newY, int newendX, int newendY, hazardMap * newhazMap);
	~astar();
	void findPath();
	int GetMap( int x, int y );
	queue * pathQueue();
	hazardMap * getHazardMap();
	bool getStatus();
};