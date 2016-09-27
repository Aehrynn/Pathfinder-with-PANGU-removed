#include "astar.h"

astar::astar()
{
	startX = 0;
	startY = 0;
	endX = 0;
	endY = 0;
	queueList = new queue();
	PATH = 50;
	STATUS = false;
}

astar::astar(int newX, int newY, int newendX, int newendY, hazardMap * newhazMap)
{
	STATUS = false;
	startX = newX;
	startY = newY;
	endX = newendX;
	endY = newendY;
	PATH = 50;
	hazMap = newhazMap;
	queueList = new queue();
	findPath();
}

astar::~astar()
{
	//delete hazMap;
	//delete queueList;
}

//Path finding algorithm

// Definitions

class MapSearchNode
{
public:
	unsigned int x;	 // the (x,y) positions of the node
	unsigned int y;	
	hazardMap * hazMap;

	MapSearchNode() { x = y = 0; }
	MapSearchNode( unsigned int px, unsigned int py) { x=px; y=py;}
	MapSearchNode( unsigned int px, unsigned int py, hazardMap * newHazMap) { x=px; y=py; hazMap=newHazMap;}

	float GoalDistanceEstimate( MapSearchNode &nodeGoal );
	bool IsGoal( MapSearchNode &nodeGoal );
	bool GetSuccessors( AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node );
	float GetCost( MapSearchNode &successor );
	bool IsSameState( MapSearchNode &rhs );
	int GetMap( int newX, int newY );

	

	void PrintNodeInfo(); 


};
int MapSearchNode::GetMap( int newX, int newY )
{

	if( newX < 0 ||
	    newX >= hazMap->getWidth() ||
		 newY < 0 ||
		 newY >= hazMap->getHeight()
	  )
	{
		return 9;	 
	}

	//return hazImage3[(y*width)+x];
	//int temp = (newY*hazMap->getWidth());
	//cout << hazMap->getMap()[(int)((newY*hazMap->getWidth())+newX)];
	//return hazMap->getMap()[(int)((newY*hazMap->getWidth())+newX)];
	return hazMap->getMap()[(int)((newX*hazMap->getWidth())+newY)];
}


bool MapSearchNode::IsSameState( MapSearchNode &rhs )
{

	// same state in a maze search is simply when (x,y) are the same
	if( (x == rhs.x) &&
		(y == rhs.y) )
	{
		return true;
	}
	else
	{
		return false;
	}

}

void MapSearchNode::PrintNodeInfo()
{
	char str[100];
	sprintf_s( str, "Node position : (%d,%d)\n", x,y );

	cout << str;
}

// Here's the heuristic function that estimates the distance from a Node
// to the Goal. 

float MapSearchNode::GoalDistanceEstimate( MapSearchNode &nodeGoal )
{
	float xd = float( ( (float)x - (float)nodeGoal.x ) );
	float yd = float( ( (float)y - (float)nodeGoal.y) );

	return ((xd*xd) + (yd*yd));

}

bool MapSearchNode::IsGoal( MapSearchNode &nodeGoal )
{

	if( (x == nodeGoal.x) &&
		(y == nodeGoal.y) )
	{
		return true;
	}

	return false;
}

// This generates the successors to the given Node. It uses a helper function called
// AddSuccessor to give the successors to the AStar class. The A* specific initialisation
// is done for each node internally, so here you just set the state information that
// is specific to the application
bool MapSearchNode::GetSuccessors( AStarSearch<MapSearchNode> *astarsearch, MapSearchNode *parent_node )
{

	int parent_x = -1; 
	int parent_y = -1; 

	if( parent_node )
	{
		parent_x = parent_node->x;
		parent_y = parent_node->y;
	}
	

	MapSearchNode NewNode;

	// push each possible move except allowing the search to go backwards

	if( (GetMap( x-1, y ) < 9) 
		&& !((parent_x == x-1) && (parent_y == y))
	  ) 
	{
		NewNode = MapSearchNode( x-1, y, hazMap);
		astarsearch->AddSuccessor( NewNode );
	}	

	if( (GetMap( x, y-1 ) < 9) 
		&& !((parent_x == x) && (parent_y == y-1))
	  ) 
	{
		NewNode = MapSearchNode( x, y-1, hazMap);
		astarsearch->AddSuccessor( NewNode );
	}	

	if( (GetMap( x+1, y ) < 9)
		&& !((parent_x == x+1) && (parent_y == y))
	  ) 
	{
		NewNode = MapSearchNode( x+1, y, hazMap);
		astarsearch->AddSuccessor( NewNode );
	}	

		
	if( (GetMap( x, y+1 ) < 9) 
		&& !((parent_x == x) && (parent_y == y+1))
		)
	{
		NewNode = MapSearchNode( x, y+1, hazMap);
		astarsearch->AddSuccessor( NewNode );
	}	

	return true;
}

// given this node, what does it cost to move to successor. In the case
// of our map the answer is the map terrain value at this node since that is 
// conceptually where we're moving

float MapSearchNode::GetCost( MapSearchNode &successor )
{
	return (float) GetMap( x, y );

}







void astar::findPath(){
	//cout << "STL A* Search implementation\n(C)2001 Justin Heyes-Jones\n";

	// Our sample problem defines the world as a 2d array representing a terrain
	// Each element contains an integer from 0 to 5 which indicates the cost 
	// of travel across the terrain. Zero means the least possible difficulty 
	// in travelling (think ice rink if you can skate) whilst 5 represents the 
	// most difficult. 9 indicates that we cannot pass.

	// Create an instance of the search class...

	AStarSearch<MapSearchNode> astarsearch;

	// Create a start state
	MapSearchNode nodeStart;
	nodeStart.x = startX;
	nodeStart.y = startY; 
	nodeStart.hazMap = hazMap;

	// Define the goal state
	MapSearchNode nodeEnd;
	nodeEnd.x = endX;							
	nodeEnd.y = endY; 
	nodeStart.hazMap = hazMap;
	
	// Set Start and goal states
	
	astarsearch.SetStartAndGoalStates( nodeStart, nodeEnd );

	unsigned int SearchState;
	unsigned int SearchSteps = 0;

	do
	{
		SearchState = astarsearch.SearchStep();

		SearchSteps++;

#if DEBUG_LISTS

		cout << "Steps:" << SearchSteps << "\n";

		int len = 0;

		cout << "Open:\n";
		MapSearchNode *p = astarsearch.GetOpenListStart();
		while( p )
		{
			len++;
#if !DEBUG_LIST_LENGTHS_ONLY			
			((MapSearchNode *)p)->PrintNodeInfo();
#endif
			p = astarsearch.GetOpenListNext();
			
		}

		cout << "Open list has " << len << " nodes\n";

		len = 0;

		cout << "Closed:\n";
		p = astarsearch.GetClosedListStart();
		while( p )
		{
			len++;
#if !DEBUG_LIST_LENGTHS_ONLY			
			p->PrintNodeInfo();
#endif			
			p = astarsearch.GetClosedListNext();
		}

		cout << "Closed list has " << len << " nodes\n";
#endif

	}
	while( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SEARCHING );

	if( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED )
	{
		//cout << "Search found goal state\n";
			STATUS = true;
			MapSearchNode *node = astarsearch.GetSolutionStart();

#if DISPLAY_SOLUTION
			cout << "Displaying solution\n";
#endif
			int steps = 0;

			//node->PrintNodeInfo();
			for( ;; )
			{
				node = astarsearch.GetSolutionNext();

				if( !node )
				{
					break;
				}
				//display route on the image
				//hazImage2[((node->y)*width)+(node->x)] = (GLubyte) (0);
				//hazImage2[(((node->y)*width)+(node->x))+1] = (GLubyte) (0);
				//hazImage2[(((node->y)*width)+(node->x))+2] = (GLubyte) (255);
				//(node->x)
				//(node->y)
				//hazMap->getHazardMap()[(int)node->y][(int)node->x]=PATH;
				queueList->addNode(node->x, node->y);
				//cout << node->x << "," << node->y << endl;
				//makeDemImage(int(height), int(width));
				//node->PrintNodeInfo();
				steps ++;
			
			};
			//makeDemImage(int(height), int(width));
			//cout << "Solution steps " << steps << endl;

			// Once you're done with the solution you can free the nodes up
			astarsearch.FreeSolutionNodes();
	
	}
	else if( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_FAILED ) 
	{
		//cout << "Search terminated. Did not find goal state\n";
		STATUS = false;
	
	}

	// Display the number of loops the search went through
	//cout << "SearchSteps : " << SearchSteps << "\n";
	
}

queue * astar::pathQueue()
{
	//Returns the queue
	//cout << "queue returned" << endl;
	return queueList;
}

hazardMap * astar::getHazardMap()
{
	return hazMap;
}

bool astar::getStatus()
{
	return STATUS;
}
//End of
