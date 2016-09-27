#include "pathlist.h"

pathList::pathList()
	{
		
	}

pathList::~pathList()
	{
		delete hazMap;
		delete waypoints;
		delete thePath;

	}
pathList::pathList(queue * newQueue, hazardMap * newhazMap)
{
	pathstatus = false;
		hazMap = newhazMap;
		waypoints = newQueue;		
		thePath = new qoqueue();
		createPath();
}


	void pathList::createPath()
	{
		
			//Start to End queue
			while(waypoints->getCurrent()->getNext()!=NULL)
			{
				//cout << waypoints->getCurrent()->getX() << endl;
				roverPath = new astar(waypoints->getCurrent()->getX(), waypoints->getCurrent()->getY(), waypoints->getCurrent()->getNext()->getX(), waypoints->getCurrent()->getNext()->getY(), hazMap);
				
				if (roverPath->getStatus()==true)
				{
					waypoints->getNext();
					thePath->addNode(roverPath ->pathQueue());
					pathstatus = roverPath->getStatus();
				}else
				{
					//Path not made
					pathstatus = roverPath->getStatus();
				}
			}
			//delete roverPath;
			
	}

	

	qoqueue * pathList::theWay()
	{
		return thePath;
	}

	bool pathList::getStatus()
	{
		return pathstatus;
	}