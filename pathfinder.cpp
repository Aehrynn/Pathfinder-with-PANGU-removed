
/*
 * Name:
 *   pathfinder.cpp
 *
 * Purpose:
 *   Controls the whole program and handles the User Interface.
 *
 * Description:
 *
 *	 Allows a User to plot points on a map and let the program find 
 *	 how to get to each point. At each stage new data will be found
 *	 and used to navigate through any hazards.
 *
 * Language:
 *   C++
 *
 * Author:
 *   Erin Maguire
 *
 * History:
 *   Forked from the real HONOURS Project to remove its associations 
 *	 with PANGU(Planet and Asteroid Natural Scene Generation Utility)
 *	 created by the University of Dundee.
 *
 *	30th July 2008 - Included deletion of arrays into the arrays class to limit duplication of deleting multi-dimensional arrays
 *	27th September 2016 - Updated to Github
 *
 * Future work:
 *   ability to choose environment to work in
 *
 * Bugs:
 *   17th June 2008 - (Fixed)Will only see hazards of hazard level 9, needs fixing so that it sees everything
 */

#include <windows.h>
#include <string.h>
#include <GL\glut.h>
#include <GL\glaux.h>
#include <GL\glui.h>
#include <stdio.h>
#include <math.h>

#include "DEM.h"
#include "hazardMap.h"
#include <cstdlib> 
#include <ctime> 
#include <iostream>
#include "socket_stuff.h"
#include "DEMarrays.h"
#include "ppm.h"
#include "ppmreader.h"
#include "rover.h"
#include "astar.h"
#include "queue.h"
#include "queueNode.h"
#include "pathlist.h"
#include "qoqueue.h"

using namespace std;


//Variables for callback functions
const int SETUP = 0;
const int SETWAYPOINT = 1;
const int STARTSTOP = 2;
const int RESET = 3;
const int QUIT = 4;
const int VIEWING = 5;
const int AUTOMATICTESTING = 7;
//
//Variables for automatic test function
char *fileName ="";
int testNumber = 10;
int errorCount = 0;
//Pie
#define PI 3.14159265
//Function List
GLdouble * GetOGLPos(int x, int y);
void setupParts();
void viewfinder();
void viewCone();
int startConnection();
void endConnection();
void autoTest();
void finishTest();
//Objects
pathList * pathObject;
ppm * imageObjectDem;
rover * roverObject;
ppmreader * imageReader;
DEM * demObject;
hazardMap * hazMap;
hazardMap * hazMap2;
hazardMap * roverMap;
hazardMap * roverMap2;
astar * newPath;
const float PATH = 50;
const float ROVER = 90;
queue * cameraQueue;
SOCKET sock;
queue * pathWayQueue;
queue * totalQueue;
qoqueue * theactualpath;
GLfloat angle = 0.0;
int POSITION = 0;
//Control Booleans
bool INITIALSETUP = true;
bool FINISHED = false;
bool START = false;
bool WAYPOINT = false;
bool simple = false;
bool ATESTING = false;
//end of - control
//double sizePixel = 0.00857143;
double sizePixel = 0.00857;
int   main_window;
GLUI *glui_h_subwindow, *glui_v_subwindow;
int font=(int)GLUT_BITMAP_8_BY_13;
string s;
//string info;
int xcoord = 0;
GLUI_EditText * xbox;//, ybox, zbox;
GLUI_EditText * ybox;
GLUI_EditText * zbox;
GLUI_EditText * hbox;
GLUI_EditText * headingbox;
GLUI_Button * finishWayPoint;
GLUI_Button * startStop;
bool connStatus = false;
ofstream rar;
float ** knownMap;

void writeString(float x, float y, void *font, string s)
{
     unsigned int c;
     glRasterPos2f(x, y);                // set position to start drawing fonts
     for (c=0; c < s.length(); c++)
          glutBitmapCharacter(font, s.at(c) ); // draw the character to the screen
}
void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();;
   //glRasterPos2i(-5, 3);
   //s = "CSC433 - Computer Graphics";
   writeString(-5, 2.5, (void *)font, s);
  // writeString(-5, -2.5, (void *)font, info);

   glRasterPos2i(-5, -2);
   //Rover Map Image
	//glDrawPixels(hazMap->getWidth(), hazMap->getHeight(), GL_RGB, GL_UNSIGNED_BYTE, roverMap->getDisplayImage());
   glDrawPixels(hazMap->getWidth(), hazMap->getHeight(), GL_RGB, GL_UNSIGNED_BYTE, roverMap2->getDisplayImage());
   
	glRasterPos2i(0, -2);
	//Complete Hazard Map image
   glDrawPixels(hazMap->getWidth(), hazMap->getHeight(), GL_RGB, GL_UNSIGNED_BYTE, hazMap->getDisplayImage());
	//glDrawPixels(hazMap->getWidth(), hazMap->getHeight(), GL_RGB, GL_UNSIGNED_BYTE, demObject->getDisplayImage());
	
   glutSwapBuffers();
}


void finishTest()
{
	ofstream rar("testlog.txt", ios::out|ios::binary|ios::ate|ios::app);
	rar << " Number of times crossed a hazard level of 9 = " << errorCount;
	rar << " Automatic Test Finshed  " ;
	rar.close();
	if(testNumber>0)
	{
		//If number of tests is not 0 then call another test
		setupParts();
								  FINISHED = false;
									START = false;
									WAYPOINT = false;
									ATESTING = false;
									autoTest();
								s = "AUTOMATIC TESTING IN PROGRESS";
	}
}

void autoTest()
{
	ATESTING = true;
	errorCount = 0;
	ofstream rar("testlog.txt", ios::out|ios::binary|ios::ate|ios::app);
	rar << "Automatic Test - " ;
	rar << testNumber << " : ";
	testNumber--;
	//Automatic testing for the algorithm
	//Set coordinates into the waypoint queue
	int testPoint = 0;
	int testX = 0;
	int testY = 0;
	while (testPoint != 2)
	{
		testX  = (rand()%((int)hazMap->getHeight()))+1;
		testY  = (rand()%((int)hazMap->getWidth()))+1;
		if (hazMap->getHazardMap()[testY][testX]<255)
					  {
						  //set waypoint
						   pathWayQueue->addNode(testX,testY);
						   rar << "Waypoint " << testPoint << " x = " << testX << " y = " << testY << " ";
						   testPoint++;

						   //Set marker on map
						   					  hazMap->getHazardMap()[testY][testX]=PATH;
					  if(testY<(hazMap->getHeight()+1))
					  {
							if(testX<(hazMap->getWidth()+1))
							{
								//bottom-right
													  hazMap->getHazardMap()[testY][testX]=PATH;

							}
							if(testX>1)
							{
								//bottom-left
													  hazMap->getHazardMap()[testY][testX]=PATH;

							}
					  }
					  if(testY>1)
					  {
							if(testX<(hazMap->getWidth()+1))
							{
								//top-right
													  hazMap->getHazardMap()[testY][testX]=PATH;

							}
							if(testX>1)
							{
								//top-left
													  hazMap->getHazardMap()[testY][testX]=PATH;

							}
							}
		
		}
	}
	rar.close();
	//From keyboard --
	 WAYPOINT = true;
				 //cout << "FINISHED WAYPOINTS SETTING" << endl;
				 s = "FINISHED WAYPOINTS SETTING - press 's' to start viewer";
				 pathObject = new pathList(pathWayQueue, roverMap);
				 if (pathObject->getStatus()==true)
				 {
				theactualpath = pathObject->theWay();
				cameraQueue = theactualpath->getCurrent()->getData();
				//hazMap->makeDisplayImage();
				roverObject = new rover(hazMap->getWidth(), hazMap->getHeight(), cameraQueue->getCurrent()->getX(),cameraQueue->getCurrent()->getY(), (demObject->getHeightMap()[(int)cameraQueue->getCurrent()->getX()][(int)cameraQueue->getCurrent()->getY()]), 0, 10, 10);

				 }else
				 {
					//path problem
					 s = "Unable to make path";
				 }
				 //end of -- keyboard
				 //Start the function
				 START = true;

}

void viewCone()
{
						//use the function
						 int maxRange= 0;// = roverObject->getRange();
						 float viewAngle = 45.0;
						 //for the rover position and heading make sure that the range isn't more than the boundaries
						 switch((int)roverObject->getHeading())
							{
						 case 0: //North
							 //North - range affected by limits of height - y
							 //if range and y of rover more than the distance between rover and limit
							 //then use a range of the most that we can - this point is going toward 0 so if y-range is > 0 then fine
							 if(((int)roverObject->getY()-roverObject->getRange())>=0)
							 {
								//then within boundaries
								 maxRange = roverObject->getRange();
							 }else
							 {
								 //cut down to allowed range
								 maxRange = (int)roverObject->getY();
								}								 
							 break;
						case 45: //NorthEast
							
							 if(((int)roverObject->getY()-roverObject->getRange())>=0)
							 {
								//then within boundaries
								 maxRange = roverObject->getRange();
							 }else
							 {
								 //cut down to allowed range
								 maxRange = (int)roverObject->getY();
								}	
							 if(((int)roverObject->getX()+roverObject->getRange())<=roverMap->getWidth())
							 {
								//then within boundaries
								 maxRange = roverObject->getRange();
							 }else
							 {
								 //cut down to allowed range
								 maxRange = (int)roverMap->getWidth() - (int)roverObject->getX();
								}
							 break;
						 case 90: //East
							 //dependant on the x being less than max width
							 if(((int)roverObject->getX()+roverObject->getRange())<=roverMap->getWidth())
							 {
								//then within boundaries
								 maxRange = roverObject->getRange();
							 }else
							 {
								 //cut down to allowed range
								 maxRange = (int)roverMap->getWidth() - (int)roverObject->getX();
								}
							 break;
							 case 135: //southEast
							 //dependant on the x being less than max width
							 if(((int)roverObject->getX()+roverObject->getRange())<=roverMap->getWidth())
							 {
								//then within boundaries
								 maxRange = roverObject->getRange();
							 }else
							 {
								 //cut down to allowed range
								 maxRange = (int)roverMap->getWidth() - (int)roverObject->getX();
								}
							  if(((int)roverObject->getY()+roverObject->getRange())<=roverMap->getHeight())
							 {
								//then within boundaries
								 maxRange = roverObject->getRange();
							 }else
							 {
								 //cut down to allowed range
								 maxRange = (int)roverMap->getHeight() - (int)roverObject->getY();
								}	
							 break;
						 case 180: //South
							 //dependant on y being less than max height
							  if(((int)roverObject->getY()+roverObject->getRange())<=roverMap->getHeight())
							 {
								//then within boundaries
								 maxRange = roverObject->getRange();
							 }else
							 {
								 //cut down to allowed range
								 maxRange = (int)roverMap->getHeight() - (int)roverObject->getY();
								}	
							 break;
							  case 225: //Southwest
							 //dependant on y being less than max height
							  if(((int)roverObject->getY()+roverObject->getRange())<=roverMap->getHeight())
							 {
								//then within boundaries
								 maxRange = roverObject->getRange();
							 }else
							 {
								 //cut down to allowed range
								 maxRange = (int)roverMap->getHeight() - (int)roverObject->getY();
								}	
							 if(((int)roverObject->getX()-roverObject->getRange())>=0)
							 {
								//then within boundaries
								 maxRange = roverObject->getRange();
							 }else
							 {
								 //cut down to allowed range
								 maxRange = (int)roverObject->getX();
								}
							 break;
						 case 270: //West
							 //dependant on x being more than 0
							 if(((int)roverObject->getX()-roverObject->getRange())>=0)
							 {
								//then within boundaries
								 maxRange = roverObject->getRange();
							 }else
							 {
								 //cut down to allowed range
								 maxRange = (int)roverObject->getX();
								}
							 break;
							 case 315: //northWest
							 //dependant on x being more than 0
							 if(((int)roverObject->getX()-roverObject->getRange())>=0)
							 {
								//then within boundaries
								 maxRange = roverObject->getRange();
							 }else
							 {
								 //cut down to allowed range
								 maxRange = (int)roverObject->getX();
								}
							  if(((int)roverObject->getY()-roverObject->getRange())>=0)
							 {
								//then within boundaries
								 maxRange = roverObject->getRange();
							 }else
							 {
								 //cut down to allowed range
								 maxRange = (int)roverObject->getY();
								}
							 break;
							}
						//cout << maxRange << endl;
						 //now that we know the maxrange we can go, this will be how many iterations through each level for the finder
						 //combinded with trignometry to find 2 triangles, we use the range from the rover as one side
						 //which points straight out in the direction and the angle of viewing, say 60 degrees, we split this into 30
						 //so that we have a similar triangle pointing out from the rover with a length of max range, 
						 // we then calculate using trig to get the other length of the triangle to know the x or y value of the corner of
						 // the triangle so we know how many iterations at each level combinded with the range levels to get all the pixels 
						 //that the rover will see in its range and angle
						 //this should be flexible as the range and angle can be set by user - as the the above simple algorithm is hardcoded to 
						 //a range of 3 pixels ahead of the rover
						// float result;
						 for(int currRange = 1; currRange<=maxRange; currRange++)
						 {
							//result 
							 int currWidth = ((tan(viewAngle*PI/180))*currRange)+0.5;
							//cout << currRange << " is " << (int)result << endl;
							/*for(int currWidth = 0; currWidth<result; currWidth++)
							{*/
								//Set the value at the location in the hazmap
								switch((int)roverObject->getHeading())
							{
									 case 0: //North
											if ((currWidth >=0) && (currWidth <=roverMap->getWidth()))
											{
												//cout << "yes" << endl;
												//If within boundaries then set
												roverMap2->getHazardMap()[(int)roverObject->getY()-currRange][(int)roverObject->getX()+currWidth] = PATH;
												roverMap2->getHazardMap()[(int)roverObject->getY()-currRange][(int)roverObject->getX()-currWidth] =PATH;
											}
											//roverMap->calculateGradient();
										 break;
										 case 45: //Northeast
											if ((currWidth >=0) && (currWidth <=roverMap->getWidth()))
											{
												//cout << "yes" << endl;
												//If within boundaries then set
												roverMap2->getHazardMap()[(int)roverObject->getY()-currRange][(int)roverObject->getX()+currWidth] =PATH;
												roverMap2->getHazardMap()[(int)roverObject->getY()-currRange][(int)roverObject->getX()-currWidth] =PATH;
											}
											if ((currWidth >=0) && (currWidth <=roverMap->getHeight()))
											{
												//cout << "yes" << endl;
												//If within boundaries then set
												roverMap2->getHazardMap()[(int)roverObject->getY()-currWidth][(int)roverObject->getX()+currRange] =PATH;
												roverMap2->getHazardMap()[(int)roverObject->getY()+currWidth][(int)roverObject->getX()+currRange] =PATH;
											}
											//roverMap->calculateGradient();
										 break;
									 case 90: //East
										if ((currWidth >=0) && (currWidth <=roverMap->getHeight()))
											{
												//cout << "yes" << endl;
												//If within boundaries then set
												roverMap2->getHazardMap()[(int)roverObject->getY()-currWidth][(int)roverObject->getX()+currRange] =PATH;
												roverMap2->getHazardMap()[(int)roverObject->getY()+currWidth][(int)roverObject->getX()+currRange] =PATH;
											}
										 break;
										 case 135: //southEast
										if ((currWidth >=0) && (currWidth <=roverMap->getHeight()))
											{
												//cout << "yes" << endl;
												//If within boundaries then set
												roverMap2->getHazardMap()[(int)roverObject->getY()-currWidth][(int)roverObject->getX()+currRange] =PATH;
												roverMap2->getHazardMap()[(int)roverObject->getY()+currWidth][(int)roverObject->getX()+currRange] =PATH;
											}
										if ((currWidth >=0) && (currWidth <=roverMap->getWidth()))
											{
												//cout << "yes" << endl;
												//If within boundaries then set
												roverMap2->getHazardMap()[(int)roverObject->getY()+currRange][(int)roverObject->getX()+currWidth] =PATH;
												roverMap2->getHazardMap()[(int)roverObject->getY()+currRange][(int)roverObject->getX()-currWidth] =PATH;
											}
										 break;
									 case 180: //South
										 if ((currWidth >=0) && (currWidth <=roverMap->getWidth()))
											{
												//cout << "yes" << endl;
												//If within boundaries then set
												roverMap2->getHazardMap()[(int)roverObject->getY()+currRange][(int)roverObject->getX()+currWidth] =PATH;
												roverMap2->getHazardMap()[(int)roverObject->getY()+currRange][(int)roverObject->getX()-currWidth] =PATH;
											}

											
										 break;
										  case 225: //Southwest
										 if ((currWidth >=0) && (currWidth <=roverMap->getWidth()))
											{
												//cout << "yes" << endl;
												//If within boundaries then set
												roverMap2->getHazardMap()[(int)roverObject->getY()+currRange][(int)roverObject->getX()+currWidth] =PATH;
												roverMap2->getHazardMap()[(int)roverObject->getY()+currRange][(int)roverObject->getX()-currWidth] =PATH;
											}
if ((currWidth >=0) && (currWidth <=roverMap->getHeight()))
											{
												//cout << "yes" << endl;
												//If within boundaries then set
												roverMap2->getHazardMap()[(int)roverObject->getY()-currWidth][(int)roverObject->getX()-currRange] =PATH;
												roverMap2->getHazardMap()[(int)roverObject->getY()+currWidth][(int)roverObject->getX()-currRange] =PATH;
											}
											
										 break;
									case 270: //West
										if ((currWidth >=0) && (currWidth <=roverMap->getHeight()))
											{
												//cout << "yes" << endl;
												//If within boundaries then set
												roverMap2->getHazardMap()[(int)roverObject->getY()-currWidth][(int)roverObject->getX()-currRange] =PATH;
												roverMap2->getHazardMap()[(int)roverObject->getY()+currWidth][(int)roverObject->getX()-currRange] =PATH;
											}
										 break;
										 case 315: //northWest
										if ((currWidth >=0) && (currWidth <=roverMap->getHeight()))
											{
												//cout << "yes" << endl;
												//If within boundaries then set
												roverMap2->getHazardMap()[(int)roverObject->getY()-currWidth][(int)roverObject->getX()-currRange] =PATH;
												roverMap2->getHazardMap()[(int)roverObject->getY()+currWidth][(int)roverObject->getX()-currRange] =PATH;
											}
										if ((currWidth >=0) && (currWidth <=roverMap->getWidth()))
											{
												//cout << "yes" << endl;
												//If within boundaries then set
												roverMap2->getHazardMap()[(int)roverObject->getY()-currRange][(int)roverObject->getX()+currWidth] =PATH;
												roverMap2->getHazardMap()[(int)roverObject->getY()-currRange][(int)roverObject->getX()-currWidth] =PATH;
											}
										 break;
							}
							//}

						 }

						 //roverMap2->calculateGradient();
						// roverMap2->updateHazards();
}
void viewfinder()
{
	//Adds the rovers field of view to its map, by using its field of view angle and its range
	//if set to simple then do it
	if(simple==true)
	{
switch((int)roverObject->getHeading())
				 {
				 case 0: //Point North
					 if(((roverObject->getX()>=1)&&(roverObject->getX()<=hazMap->getWidth()-1))&&((roverObject->getY()>=1)&&(roverObject->getY()<=hazMap->getHeight()-1)))
					 {
						//within constraints of 1 pixel border - pixels
						 roverMap->getHazardMap()[(int)roverObject->getY()-1][(int)roverObject->getX()+1] = hazMap->getHazardMap()[(int)roverObject->getY()-1][(int)roverObject->getX()+1];
						 roverMap->getHazardMap()[(int)roverObject->getY()-1][(int)roverObject->getX()] = hazMap->getHazardMap()[(int)roverObject->getY()-1][(int)roverObject->getX()];
						 roverMap->getHazardMap()[(int)roverObject->getY()-1][(int)roverObject->getX()-1] = hazMap->getHazardMap()[(int)roverObject->getY()-1][(int)roverObject->getX()-1];
						 //cout << roverMap->getHazardMap()[(int)roverObject->getY()-1][(int)roverObject->getX()+1] << endl;
					 }
					 if(((roverObject->getX()>=2)&&(roverObject->getX()<=hazMap->getWidth()-2))&&((roverObject->getY()>=2)&&(roverObject->getY()<=hazMap->getHeight()-2)))
					 {
						//within constraints of 2 pixel border - 5 pixels
						roverMap->getHazardMap()[(int)roverObject->getY()-2][(int)roverObject->getX()+1] =  hazMap->getHazardMap()[(int)roverObject->getY()-2][(int)roverObject->getX()+1];
						roverMap->getHazardMap()[(int)roverObject->getY()-2][(int)roverObject->getX()] =  hazMap->getHazardMap()[(int)roverObject->getY()-2][(int)roverObject->getX()];
						roverMap->getHazardMap()[(int)roverObject->getY()-2][(int)roverObject->getX()-1] =  hazMap->getHazardMap()[(int)roverObject->getY()-2][(int)roverObject->getX()-1];
						roverMap->getHazardMap()[(int)roverObject->getY()-2][(int)roverObject->getX()+2] =  hazMap->getHazardMap()[(int)roverObject->getY()-2][(int)roverObject->getX()+2];
						roverMap->getHazardMap()[(int)roverObject->getY()-2][(int)roverObject->getX()-2] =  hazMap->getHazardMap()[(int)roverObject->getY()-2][(int)roverObject->getX()-2];
					 }
					 if(((roverObject->getX()>=3)&&(roverObject->getX()<=hazMap->getWidth()-3))&&((roverObject->getY()>=3)&&(roverObject->getY()<=hazMap->getHeight()-3)))
					 {
						//within constraints of 3 pixel border - 5 pixels
						 roverMap->getHazardMap()[(int)roverObject->getY()-3][(int)roverObject->getX()+1] =  hazMap->getHazardMap()[(int)roverObject->getY()-3][(int)roverObject->getX()+1];
						roverMap->getHazardMap()[(int)roverObject->getY()-3][(int)roverObject->getX()] =  hazMap->getHazardMap()[(int)roverObject->getY()-3][(int)roverObject->getX()];
						roverMap->getHazardMap()[(int)roverObject->getY()-3][(int)roverObject->getX()-1] =  hazMap->getHazardMap()[(int)roverObject->getY()-3][(int)roverObject->getX()-1];
						roverMap->getHazardMap()[(int)roverObject->getY()-3][(int)roverObject->getX()+2] =  hazMap->getHazardMap()[(int)roverObject->getY()-3][(int)roverObject->getX()+2];
						roverMap->getHazardMap()[(int)roverObject->getY()-3][(int)roverObject->getX()-2] =  hazMap->getHazardMap()[(int)roverObject->getY()-3][(int)roverObject->getX()-2];
					
					 }
					 roverMap->calculateGradient();
					 break;
					 case 90: //Point East
					 if(((roverObject->getX()>=1)&&(roverObject->getX()<=hazMap->getWidth()-1))&&((roverObject->getY()>=1)&&(roverObject->getY()<=hazMap->getHeight()-1)))
					 {
						//within constraints of 1 pixel border - pixels
						 roverMap->getHazardMap()[(int)roverObject->getY()-1][(int)roverObject->getX()+1] = hazMap->getHazardMap()[(int)roverObject->getY()-1][(int)roverObject->getX()+1];
						 roverMap->getHazardMap()[(int)roverObject->getY()][(int)roverObject->getX()+1] = hazMap->getHazardMap()[(int)roverObject->getY()][(int)roverObject->getX()+1];
						 roverMap->getHazardMap()[(int)roverObject->getY()+1][(int)roverObject->getX()+1] = hazMap->getHazardMap()[(int)roverObject->getY()+1][(int)roverObject->getX()+1];
						 //cout << roverMap->getHazardMap()[(int)roverObject->getY()-1][(int)roverObject->getX()+1] << endl;
					 }
					 if(((roverObject->getX()>=2)&&(roverObject->getX()<=hazMap->getWidth()-2))&&((roverObject->getY()>=2)&&(roverObject->getY()<=hazMap->getHeight()-2)))
					 {
						//within constraints of 2 pixel border - 5 pixels
						roverMap->getHazardMap()[(int)roverObject->getY()-2][(int)roverObject->getX()+2] =  hazMap->getHazardMap()[(int)roverObject->getY()-2][(int)roverObject->getX()+2];
						roverMap->getHazardMap()[(int)roverObject->getY()][(int)roverObject->getX()+2] =  hazMap->getHazardMap()[(int)roverObject->getY()][(int)roverObject->getX()+2];
						roverMap->getHazardMap()[(int)roverObject->getY()+2][(int)roverObject->getX()+2] =  hazMap->getHazardMap()[(int)roverObject->getY()+2][(int)roverObject->getX()+2];
						roverMap->getHazardMap()[(int)roverObject->getY()-1][(int)roverObject->getX()+2] =  hazMap->getHazardMap()[(int)roverObject->getY()-1][(int)roverObject->getX()+2];
						roverMap->getHazardMap()[(int)roverObject->getY()+1][(int)roverObject->getX()+2] =  hazMap->getHazardMap()[(int)roverObject->getY()+2][(int)roverObject->getX()+2];
					 }
					 if(((roverObject->getX()>=3)&&(roverObject->getX()<=hazMap->getWidth()-3))&&((roverObject->getY()>=3)&&(roverObject->getY()<=hazMap->getHeight()-3)))
					 {
						//within constraints of 3 pixel border - 5 pixels
						 roverMap->getHazardMap()[(int)roverObject->getY()-2][(int)roverObject->getX()+3] =  hazMap->getHazardMap()[(int)roverObject->getY()-2][(int)roverObject->getX()+3];
						roverMap->getHazardMap()[(int)roverObject->getY()][(int)roverObject->getX()+3] =  hazMap->getHazardMap()[(int)roverObject->getY()][(int)roverObject->getX()+3];
						roverMap->getHazardMap()[(int)roverObject->getY()+2][(int)roverObject->getX()+3] =  hazMap->getHazardMap()[(int)roverObject->getY()+2][(int)roverObject->getX()+3];
						roverMap->getHazardMap()[(int)roverObject->getY()-1][(int)roverObject->getX()+3] =  hazMap->getHazardMap()[(int)roverObject->getY()-1][(int)roverObject->getX()+3];
						roverMap->getHazardMap()[(int)roverObject->getY()+1][(int)roverObject->getX()+3] =  hazMap->getHazardMap()[(int)roverObject->getY()+1][(int)roverObject->getX()+3];
					
					 }
					 roverMap->calculateGradient();
					 break;
					 case 180: //Point South
					 if(((roverObject->getX()>=1)&&(roverObject->getX()<=hazMap->getWidth()-1))&&((roverObject->getY()>=1)&&(roverObject->getY()<=hazMap->getHeight()-1)))
					 {
						//within constraints of 1 pixel border - pixels
						 roverMap->getHazardMap()[(int)roverObject->getY()+1][(int)roverObject->getX()+1] = hazMap->getHazardMap()[(int)roverObject->getY()+1][(int)roverObject->getX()+1];
						 roverMap->getHazardMap()[(int)roverObject->getY()+1][(int)roverObject->getX()] = hazMap->getHazardMap()[(int)roverObject->getY()+1][(int)roverObject->getX()];
						 roverMap->getHazardMap()[(int)roverObject->getY()+1][(int)roverObject->getX()-1] = hazMap->getHazardMap()[(int)roverObject->getY()+1][(int)roverObject->getX()-1];
						 //cout << roverMap->getHazardMap()[(int)roverObject->getY()-1][(int)roverObject->getX()+1] << endl;
					 }
					 if(((roverObject->getX()>=2)&&(roverObject->getX()<=hazMap->getWidth()-2))&&((roverObject->getY()>=2)&&(roverObject->getY()<=hazMap->getHeight()-2)))
					 {
						//within constraints of 2 pixel border - 5 pixels
						roverMap->getHazardMap()[(int)roverObject->getY()+2][(int)roverObject->getX()+1] =  hazMap->getHazardMap()[(int)roverObject->getY()+2][(int)roverObject->getX()+1];
						roverMap->getHazardMap()[(int)roverObject->getY()+2][(int)roverObject->getX()] =  hazMap->getHazardMap()[(int)roverObject->getY()+2][(int)roverObject->getX()];
						roverMap->getHazardMap()[(int)roverObject->getY()+2][(int)roverObject->getX()-1] =  hazMap->getHazardMap()[(int)roverObject->getY()+2][(int)roverObject->getX()-1];
						roverMap->getHazardMap()[(int)roverObject->getY()+2][(int)roverObject->getX()+2] =  hazMap->getHazardMap()[(int)roverObject->getY()+2][(int)roverObject->getX()+2];
						roverMap->getHazardMap()[(int)roverObject->getY()+2][(int)roverObject->getX()-2] =  hazMap->getHazardMap()[(int)roverObject->getY()+2][(int)roverObject->getX()-2];
					 }
					 if(((roverObject->getX()>=3)&&(roverObject->getX()<=hazMap->getWidth()-3))&&((roverObject->getY()>=3)&&(roverObject->getY()<=hazMap->getHeight()-3)))
					 {
						//within constraints of 3 pixel border - 5 pixels
						 roverMap->getHazardMap()[(int)roverObject->getY()+3][(int)roverObject->getX()+1] =  hazMap->getHazardMap()[(int)roverObject->getY()+3][(int)roverObject->getX()+1];
						roverMap->getHazardMap()[(int)roverObject->getY()+3][(int)roverObject->getX()] =  hazMap->getHazardMap()[(int)roverObject->getY()+3][(int)roverObject->getX()];
						roverMap->getHazardMap()[(int)roverObject->getY()+3][(int)roverObject->getX()-1] =  hazMap->getHazardMap()[(int)roverObject->getY()+3][(int)roverObject->getX()-1];
						roverMap->getHazardMap()[(int)roverObject->getY()+3][(int)roverObject->getX()+2] =  hazMap->getHazardMap()[(int)roverObject->getY()+3][(int)roverObject->getX()+2];
						roverMap->getHazardMap()[(int)roverObject->getY()+3][(int)roverObject->getX()-2] =  hazMap->getHazardMap()[(int)roverObject->getY()+3][(int)roverObject->getX()-2];
					
					 }
					 roverMap->calculateGradient();
					 break;
					  case 270: //Point west
					 if(((roverObject->getX()>=1)&&(roverObject->getX()<=hazMap->getWidth()-1))&&((roverObject->getY()>=1)&&(roverObject->getY()<=hazMap->getHeight()-1)))
					 {
						//within constraints of 1 pixel border - pixels
						 roverMap->getHazardMap()[(int)roverObject->getY()-1][(int)roverObject->getX()-1] = hazMap->getHazardMap()[(int)roverObject->getY()-1][(int)roverObject->getX()-1];
						 roverMap->getHazardMap()[(int)roverObject->getY()][(int)roverObject->getX()-1] = hazMap->getHazardMap()[(int)roverObject->getY()][(int)roverObject->getX()-1];
						 roverMap->getHazardMap()[(int)roverObject->getY()+1][(int)roverObject->getX()-1] = hazMap->getHazardMap()[(int)roverObject->getY()+1][(int)roverObject->getX()-1];
						 //cout << roverMap->getHazardMap()[(int)roverObject->getY()-1][(int)roverObject->getX()+1] << endl;
					 }
					 if(((roverObject->getX()>=2)&&(roverObject->getX()<=hazMap->getWidth()-2))&&((roverObject->getY()>=2)&&(roverObject->getY()<=hazMap->getHeight()-2)))
					 {
						//within constraints of 2 pixel border - 5 pixels
						roverMap->getHazardMap()[(int)roverObject->getY()-2][(int)roverObject->getX()-2] =  hazMap->getHazardMap()[(int)roverObject->getY()-2][(int)roverObject->getX()-2];
						roverMap->getHazardMap()[(int)roverObject->getY()][(int)roverObject->getX()-2] =  hazMap->getHazardMap()[(int)roverObject->getY()][(int)roverObject->getX()-2];
						roverMap->getHazardMap()[(int)roverObject->getY()+2][(int)roverObject->getX()-2] =  hazMap->getHazardMap()[(int)roverObject->getY()+2][(int)roverObject->getX()-2];
						roverMap->getHazardMap()[(int)roverObject->getY()-1][(int)roverObject->getX()-2] =  hazMap->getHazardMap()[(int)roverObject->getY()-1][(int)roverObject->getX()-2];
						roverMap->getHazardMap()[(int)roverObject->getY()+1][(int)roverObject->getX()-2] =  hazMap->getHazardMap()[(int)roverObject->getY()+2][(int)roverObject->getX()-2];
					 }
					 if(((roverObject->getX()>=3)&&(roverObject->getX()<=hazMap->getWidth()-3))&&((roverObject->getY()>=3)&&(roverObject->getY()<=hazMap->getHeight()-3)))
					 {
						//within constraints of 3 pixel border - 5 pixels
						 roverMap->getHazardMap()[(int)roverObject->getY()-2][(int)roverObject->getX()-3] =  hazMap->getHazardMap()[(int)roverObject->getY()-2][(int)roverObject->getX()-3];
						roverMap->getHazardMap()[(int)roverObject->getY()][(int)roverObject->getX()-3] =  hazMap->getHazardMap()[(int)roverObject->getY()][(int)roverObject->getX()-3];
						roverMap->getHazardMap()[(int)roverObject->getY()+2][(int)roverObject->getX()-3] =  hazMap->getHazardMap()[(int)roverObject->getY()+2][(int)roverObject->getX()-3];
						roverMap->getHazardMap()[(int)roverObject->getY()-1][(int)roverObject->getX()-3] =  hazMap->getHazardMap()[(int)roverObject->getY()-1][(int)roverObject->getX()-3];
						roverMap->getHazardMap()[(int)roverObject->getY()+1][(int)roverObject->getX()-3] =  hazMap->getHazardMap()[(int)roverObject->getY()+1][(int)roverObject->getX()-3];
					
					 }
					 roverMap->calculateGradient();
					 break;
				 }
				 }else
					 {
					//use the function
						 int maxRange= 0;// = roverObject->getRange();
						 float viewAngle = 45.0;
						 //for the rover position and heading make sure that the range isn't more than the boundaries
						 switch((int)roverObject->getHeading())
							{
						 case 0: //North
							 //North - range affected by limits of height - y
							 //if range and y of rover more than the distance between rover and limit
							 //then use a range of the most that we can - this point is going toward 0 so if y-range is > 0 then fine
							 if(((int)roverObject->getY()-roverObject->getRange())>=0)
							 {
								//then within boundaries
								 maxRange = roverObject->getRange();
							 }else
							 {
								 //cut down to allowed range
								 maxRange = (int)roverObject->getY();
								}								 
							 break;
						case 45: //NorthEast
							
							 if(((int)roverObject->getY()-roverObject->getRange())>=0)
							 {
								//then within boundaries
								 maxRange = roverObject->getRange();
							 }else
							 {
								 //cut down to allowed range
								 maxRange = (int)roverObject->getY();
								}	
							 if(((int)roverObject->getX()+roverObject->getRange())<=roverMap->getWidth())
							 {
								//then within boundaries
								 maxRange = roverObject->getRange();
							 }else
							 {
								 //cut down to allowed range
								 maxRange = (int)roverMap->getWidth() - (int)roverObject->getX();
								}
							 break;
						 case 90: //East
							 //dependant on the x being less than max width
							 if(((int)roverObject->getX()+roverObject->getRange())<=roverMap->getWidth())
							 {
								//then within boundaries
								 maxRange = roverObject->getRange();
							 }else
							 {
								 //cut down to allowed range
								 maxRange = (int)roverMap->getWidth() - (int)roverObject->getX();
								}
							 break;
							 case 135: //southEast
							 //dependant on the x being less than max width
							 if(((int)roverObject->getX()+roverObject->getRange())<=roverMap->getWidth())
							 {
								//then within boundaries
								 maxRange = roverObject->getRange();
							 }else
							 {
								 //cut down to allowed range
								 maxRange = (int)roverMap->getWidth() - (int)roverObject->getX();
								}
							  if(((int)roverObject->getY()+roverObject->getRange())<=roverMap->getHeight())
							 {
								//then within boundaries
								 maxRange = roverObject->getRange();
							 }else
							 {
								 //cut down to allowed range
								 maxRange = (int)roverMap->getHeight() - (int)roverObject->getY();
								}	
							 break;
						 case 180: //South
							 //dependant on y being less than max height
							  if(((int)roverObject->getY()+roverObject->getRange())<=roverMap->getHeight())
							 {
								//then within boundaries
								 maxRange = roverObject->getRange();
							 }else
							 {
								 //cut down to allowed range
								 maxRange = (int)roverMap->getHeight() - (int)roverObject->getY();
								}	
							 break;
							  case 225: //Southwest
							 //dependant on y being less than max height
							  if(((int)roverObject->getY()+roverObject->getRange())<=roverMap->getHeight())
							 {
								//then within boundaries
								 maxRange = roverObject->getRange();
							 }else
							 {
								 //cut down to allowed range
								 maxRange = (int)roverMap->getHeight() - (int)roverObject->getY();
								}	
							 if(((int)roverObject->getX()-roverObject->getRange())>=0)
							 {
								//then within boundaries
								 maxRange = roverObject->getRange();
							 }else
							 {
								 //cut down to allowed range
								 maxRange = (int)roverObject->getX();
								}
							 break;
						 case 270: //West
							 //dependant on x being more than 0
							 if(((int)roverObject->getX()-roverObject->getRange())>=0)
							 {
								//then within boundaries
								 maxRange = roverObject->getRange();
							 }else
							 {
								 //cut down to allowed range
								 maxRange = (int)roverObject->getX();
								}
							 break;
							 case 315: //northWest
							 //dependant on x being more than 0
							 if(((int)roverObject->getX()-roverObject->getRange())>=0)
							 {
								//then within boundaries
								 maxRange = roverObject->getRange();
							 }else
							 {
								 //cut down to allowed range
								 maxRange = (int)roverObject->getX();
								}
							  if(((int)roverObject->getY()-roverObject->getRange())>=0)
							 {
								//then within boundaries
								 maxRange = roverObject->getRange();
							 }else
							 {
								 //cut down to allowed range
								 maxRange = (int)roverObject->getY();
								}
							 break;
							}
						//cout << maxRange << endl;
						 //now that we know the maxrange we can go, this will be how many iterations through each level for the finder
						 //combinded with trignometry to find 2 triangles, we use the range from the rover as one side
						 //which points straight out in the direction and the angle of viewing, say 60 degrees, we split this into 30
						 //so that we have a similar triangle pointing out from the rover with a length of max range, 
						 // we then calculate using trig to get the other length of the triangle to know the x or y value of the corner of
						 // the triangle so we know how many iterations at each level combinded with the range levels to get all the pixels 
						 //that the rover will see in its range and angle
						 //this should be flexible as the range and angle can be set by user - as the the above simple algorithm is hardcoded to 
						 //a range of 3 pixels ahead of the rover
						 float result;
						 for(int currRange = 1; currRange<=maxRange; currRange++)
						 {
							result = ((tan(viewAngle*PI/180))*currRange)+0.5;
							//cout << currRange << " is " << (int)result << endl;
							for(int currWidth = 0; currWidth<result; currWidth++)
							{
								//Set the value at the location in the hazmap
								switch((int)roverObject->getHeading())
							{
									 case 0: //North
											if ((currWidth >=0) && (currWidth <=roverMap->getWidth()))
											{
												//cout << "yes" << endl;
												//If within boundaries then set
												roverMap->getHazardMap()[(int)roverObject->getY()-currRange][(int)roverObject->getX()+currWidth] =  hazMap2->getHazardMap()[(int)roverObject->getY()-currRange][(int)roverObject->getX()+currWidth];
												roverMap->getHazardMap()[(int)roverObject->getY()-currRange][(int)roverObject->getX()-currWidth] =  hazMap2->getHazardMap()[(int)roverObject->getY()-currRange][(int)roverObject->getX()-currWidth];
												//add to knownMap
												/*knownMap[(int)roverObject->getY()-currRange][(int)roverObject->getX()+currWidth] =  hazMap2->getHazardMap()[(int)roverObject->getY()-currRange][(int)roverObject->getX()+currWidth];
												knownMap[(int)roverObject->getY()-currRange][(int)roverObject->getX()-currWidth] =  hazMap2->getHazardMap()[(int)roverObject->getY()-currRange][(int)roverObject->getX()-currWidth];
												roverMap->setHazardMap(knownMap);*/
											}
											//roverMap->calculateGradient();
										 break;
										 case 45: //Northeast
											if ((currWidth >0) && (currWidth <roverMap->getWidth()))
											{
												//cout << "yes" << endl;
												//If within boundaries then set
												roverMap->getHazardMap()[(int)roverObject->getY()-currRange][(int)roverObject->getX()+currWidth] =  hazMap2->getHazardMap()[(int)roverObject->getY()-currRange][(int)roverObject->getX()+currWidth];
												roverMap->getHazardMap()[(int)roverObject->getY()-currRange][(int)roverObject->getX()-currWidth] =  hazMap2->getHazardMap()[(int)roverObject->getY()-currRange][(int)roverObject->getX()-currWidth];
											}
											if ((currWidth >0) && (currWidth <roverMap->getHeight()))
											{
												//cout << "yes" << endl;
												//If within boundaries then set
												roverMap->getHazardMap()[(int)roverObject->getY()-currWidth][(int)roverObject->getX()+currRange] =  hazMap2->getHazardMap()[(int)roverObject->getY()-currWidth][(int)roverObject->getX()+currRange];
												roverMap->getHazardMap()[(int)roverObject->getY()+currWidth][(int)roverObject->getX()+currRange] =  hazMap2->getHazardMap()[(int)roverObject->getY()+currWidth][(int)roverObject->getX()+currRange];
											}
											//roverMap->calculateGradient();
										 break;
									 case 90: //East
										if ((currWidth >=0) && (currWidth <=roverMap->getHeight()))
											{
												//cout << "yes" << endl;
												//If within boundaries then set
												roverMap->getHazardMap()[(int)roverObject->getY()-currWidth][(int)roverObject->getX()+currRange] =  hazMap2->getHazardMap()[(int)roverObject->getY()-currWidth][(int)roverObject->getX()+currRange];
												roverMap->getHazardMap()[(int)roverObject->getY()+currWidth][(int)roverObject->getX()+currRange] =  hazMap2->getHazardMap()[(int)roverObject->getY()+currWidth][(int)roverObject->getX()+currRange];
											}
										 break;
										 case 135: //southEast
										if ((currWidth >=0) && (currWidth <=roverMap->getHeight()))
											{
												//cout << "yes" << endl;
												//If within boundaries then set
												roverMap->getHazardMap()[(int)roverObject->getY()-currWidth][(int)roverObject->getX()+currRange] =  hazMap2->getHazardMap()[(int)roverObject->getY()-currWidth][(int)roverObject->getX()+currRange];
												roverMap->getHazardMap()[(int)roverObject->getY()+currWidth][(int)roverObject->getX()+currRange] =  hazMap2->getHazardMap()[(int)roverObject->getY()+currWidth][(int)roverObject->getX()+currRange];
											}
										if ((currWidth >=0) && (currWidth <=roverMap->getWidth()))
											{
												//cout << "yes" << endl;
												//If within boundaries then set
												roverMap->getHazardMap()[(int)roverObject->getY()+currRange][(int)roverObject->getX()+currWidth] =  hazMap2->getHazardMap()[(int)roverObject->getY()+currRange][(int)roverObject->getX()+currWidth];
												roverMap->getHazardMap()[(int)roverObject->getY()+currRange][(int)roverObject->getX()-currWidth] =  hazMap2->getHazardMap()[(int)roverObject->getY()+currRange][(int)roverObject->getX()-currWidth];
											}
										 break;
									 case 180: //South
										 if ((currWidth >=0) && (currWidth <=roverMap->getWidth()))
											{
												//cout << "yes" << endl;
												//If within boundaries then set
												roverMap->getHazardMap()[(int)roverObject->getY()+currRange][(int)roverObject->getX()+currWidth] =  hazMap2->getHazardMap()[(int)roverObject->getY()+currRange][(int)roverObject->getX()+currWidth];
												roverMap->getHazardMap()[(int)roverObject->getY()+currRange][(int)roverObject->getX()-currWidth] =  hazMap2->getHazardMap()[(int)roverObject->getY()+currRange][(int)roverObject->getX()-currWidth];
											}

											
										 break;
										  case 225: //Southwest
										 if ((currWidth >=0) && (currWidth <=roverMap->getWidth()))
											{
												//cout << "yes" << endl;
												//If within boundaries then set
												roverMap->getHazardMap()[(int)roverObject->getY()+currRange][(int)roverObject->getX()+currWidth] =  hazMap2->getHazardMap()[(int)roverObject->getY()+currRange][(int)roverObject->getX()+currWidth];
												roverMap->getHazardMap()[(int)roverObject->getY()+currRange][(int)roverObject->getX()-currWidth] =  hazMap2->getHazardMap()[(int)roverObject->getY()+currRange][(int)roverObject->getX()-currWidth];
											}
if ((currWidth >=0) && (currWidth <=roverMap->getHeight()))
											{
												//cout << "yes" << endl;
												//If within boundaries then set
												roverMap->getHazardMap()[(int)roverObject->getY()-currWidth][(int)roverObject->getX()-currRange] =  hazMap2->getHazardMap()[(int)roverObject->getY()-currWidth][(int)roverObject->getX()-currRange];
												roverMap->getHazardMap()[(int)roverObject->getY()+currWidth][(int)roverObject->getX()-currRange] =  hazMap2->getHazardMap()[(int)roverObject->getY()+currWidth][(int)roverObject->getX()-currRange];
											}
											
										 break;
									case 270: //West
										if ((currWidth >=0) && (currWidth <=roverMap->getHeight()))
											{
												//cout << "yes" << endl;
												//If within boundaries then set
												roverMap->getHazardMap()[(int)roverObject->getY()-currWidth][(int)roverObject->getX()-currRange] =  hazMap2->getHazardMap()[(int)roverObject->getY()-currWidth][(int)roverObject->getX()-currRange];
												roverMap->getHazardMap()[(int)roverObject->getY()+currWidth][(int)roverObject->getX()-currRange] =  hazMap2->getHazardMap()[(int)roverObject->getY()+currWidth][(int)roverObject->getX()-currRange];
											}
										 break;
										 case 315: //northWest
										if ((currWidth >=0) && (currWidth <=roverMap->getHeight()))
											{
												//cout << "yes" << endl;
												//If within boundaries then set
												roverMap->getHazardMap()[(int)roverObject->getY()-currWidth][(int)roverObject->getX()-currRange] =  hazMap2->getHazardMap()[(int)roverObject->getY()-currWidth][(int)roverObject->getX()-currRange];
												roverMap->getHazardMap()[(int)roverObject->getY()+currWidth][(int)roverObject->getX()-currRange] =  hazMap2->getHazardMap()[(int)roverObject->getY()+currWidth][(int)roverObject->getX()-currRange];
											}
										if ((currWidth >=0) && (currWidth <=roverMap->getWidth()))
											{
												//cout << "yes" << endl;
												//If within boundaries then set
												roverMap->getHazardMap()[(int)roverObject->getY()-currRange][(int)roverObject->getX()+currWidth] =  hazMap2->getHazardMap()[(int)roverObject->getY()-currRange][(int)roverObject->getX()+currWidth];
												roverMap->getHazardMap()[(int)roverObject->getY()-currRange][(int)roverObject->getX()-currWidth] =  hazMap2->getHazardMap()[(int)roverObject->getY()-currRange][(int)roverObject->getX()-currWidth];
											}
										 break;
							}
							}

						 }
						 //
						 
						 //roverMap->calculateGradient();
						 roverMap->updateHazards();
				 }
}

void init(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_FILL);
   
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (w <= h)
      glOrtho(-3.0, 3.0, -3.0*(GLfloat)h/(GLfloat)w, 
              3.0*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
   else
      glOrtho(-3.0*(GLfloat)w/(GLfloat)h, 
              3.0*(GLfloat)w/(GLfloat)h, -3.0, 3.0, -10.0, 10.0);
   //GLUI_Master.auto_set_viewport();
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
		 
         break;
		 case 's':
          if (WAYPOINT == true)
		  {
		  if ( START==false)
			 {
				 START = true;
				 //cout << "Viewing Started" << endl;
				 s = "Viewing Started";
				 glutPostRedisplay();
			 }else if ( START == true)
			 {
				 START = false;
				 //cout << "Viewing Stopped" << endl;
				 s = "Viewing Stopped";
				 glutPostRedisplay();
			 }
		  }else{
			  s = "Set waypoint(s) before starting, press waypoint button to finish setting waypoints";
			  glutPostRedisplay();
		  }
	
         break;
		 case 'w':
			 if (( WAYPOINT==false) && (pathWayQueue->getCurrent()->getNext()!=NULL))
			 {
				 WAYPOINT = true;
				 //cout << "FINISHED WAYPOINTS SETTING" << endl;
				 s = "FINISHED WAYPOINTS SETTING - press 's' to start viewer";
				 pathObject = new pathList(pathWayQueue, roverMap);
				 if (pathObject->getStatus()==true)
				 {
				theactualpath = pathObject->theWay();
				cameraQueue = theactualpath->getCurrent()->getData();
				//hazMap->makeDisplayImage();
				roverObject = new rover(hazMap->getWidth(), hazMap->getHeight(), cameraQueue->getCurrent()->getX(),cameraQueue->getCurrent()->getY(), (demObject->getHeightMap()[(int)cameraQueue->getCurrent()->getX()][(int)cameraQueue->getCurrent()->getY()]), 0, 10, 10);

				 }else
				 {
					//path problem
					 s = "Unable to make path";
				 }
				glutPostRedisplay();
			 }
	
         break;
   }
}


void mouse (int button, int state, int x, int y)
{
  switch (button)  {
    case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
		  if (START == false)
		  {
			  if (WAYPOINT == false)
			  {
				  GLdouble *testVector = GetOGLPos(x, y);
				  if (((testVector[0]>=0)&&(testVector[0]<=(sizePixel*hazMap->getWidth())))&&((testVector[1]>=-2)&&(testVector[1]<=(sizePixel*hazMap->getHeight()))))
				  {
					  if (hazMap->getHazardMap()[(int)((hazMap->getWidth()-((((testVector[1]+2)/(sizePixel*hazMap->getWidth()))*hazMap->getWidth()))))][(int)((testVector[0]/(sizePixel*hazMap->getWidth()))*hazMap->getWidth())]<255)
					  {
					  hazMap->getHazardMap()[(int)((hazMap->getWidth()-((((testVector[1]+2)/(sizePixel*hazMap->getWidth()))*hazMap->getWidth()))))][(int)((testVector[0]/(sizePixel*hazMap->getWidth()))*hazMap->getWidth())]=PATH;
					  if(((hazMap->getWidth()-((((testVector[1]+2)/(sizePixel*hazMap->getWidth()))*hazMap->getWidth()))))<(hazMap->getHeight()+1))
					  {
							if(((testVector[0]/(sizePixel*hazMap->getWidth()))*hazMap->getWidth())<(hazMap->getWidth()+1))
							{
								//bottom-right
													  hazMap->getHazardMap()[(int)(((hazMap->getWidth()-((((testVector[1]+2)/(sizePixel*hazMap->getWidth()))*hazMap->getWidth()))))+1)][(int)(((testVector[0]/(sizePixel*hazMap->getWidth()))*hazMap->getWidth())+1)]=PATH;

							}
							if(((testVector[0]/(sizePixel*hazMap->getWidth()))*hazMap->getWidth())>1)
							{
								//bottom-left
													  hazMap->getHazardMap()[(int)(((hazMap->getWidth()-((((testVector[1]+2)/(sizePixel*hazMap->getWidth()))*hazMap->getWidth()))))+1)][(int)(((testVector[0]/(sizePixel*hazMap->getWidth()))*hazMap->getWidth())-1)]=PATH;

							}
					  }
					  if(((hazMap->getWidth()-((((testVector[1]+2)/(sizePixel*hazMap->getWidth()))*hazMap->getWidth()))))>1)
					  {
							if(((testVector[0]/(sizePixel*hazMap->getWidth()))*hazMap->getWidth())<(hazMap->getWidth()+1))
							{
								//top-right
													  hazMap->getHazardMap()[(int)(((hazMap->getWidth()-((((testVector[1]+2)/(sizePixel*hazMap->getWidth()))*hazMap->getWidth()))))-1)][(int)(((testVector[0]/(sizePixel*hazMap->getWidth()))*hazMap->getWidth())+1)]=PATH;

							}
							if(((testVector[0]/(sizePixel*hazMap->getWidth()))*hazMap->getWidth())>1)
							{
								//top-left
													  hazMap->getHazardMap()[(int)(((hazMap->getWidth()-((((testVector[1]+2)/(sizePixel*hazMap->getWidth()))*hazMap->getWidth()))))-1)][(int)(((testVector[0]/(sizePixel*hazMap->getWidth()))*hazMap->getWidth())-1)]=PATH;

							}
					  }

			 pathWayQueue->addNode(((testVector[0]/(sizePixel*hazMap->getWidth()))*hazMap->getWidth()),(hazMap->getWidth()-((((testVector[1]+2)/(sizePixel*hazMap->getWidth()))*hazMap->getWidth()))));
			  }
				  }
		  }
		  }
		  glutPostRedisplay();
		}
	
      break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
		  
		  glutPostRedisplay();
		}
      break;
    default:
      break;
  }
}

GLdouble * GetOGLPos(int x, int y)
{
	//Used to get the opengl coordinates from the window coordinates
	//From NeHe article 13 Using gluUnProject - http://nehe.gamedev.net/data/articles/article.asp?article=13#4
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	GLdouble posX, posY, posZ;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
GLdouble * varray = new GLdouble [3];
varray[0] = posX;
varray[1] = posY;
varray[2] = posZ;
	return varray;
}

void idle(void)
{
	glutSetWindow(main_window);
	if((START==true)&&(FINISHED==false)&&(WAYPOINT==true))
	{
		 if (cameraQueue->getCurrent() == cameraQueue->getLast())
			 {
				 //at end of a queue
				 if (theactualpath->getCurrent()->getNext() == NULL)
				 {
					 //If next in line is null then end
					 FINISHED = true;
					 s = "Traversal Complete - Press Reset Button to set new waypoint(s)";
					 if (ATESTING == true)
					 {
						//Close log file
						finishTest();
					 }
				 }else 
				 {
					 //if there is another queue next then use it
					 theactualpath->getNext();
					 cameraQueue = theactualpath->getCurrent()->getData();	
					 newPath = new astar((int)roverObject->getX(), (int)roverObject->getY(), cameraQueue->getLast()->getX(), cameraQueue->getLast()->getY(), roverMap);
				cameraQueue = newPath->pathQueue();
				 }
			 }else if (cameraQueue->getCurrent()!=NULL)
			 {
				 //if the next is not null, set as new position
				 cameraQueue->getNext();
				roverObject->setXY(cameraQueue->getCurrent()->getX(),cameraQueue->getCurrent()->getY());
				 roverObject->setZ(demObject->getHeightMap()[(int)cameraQueue->getCurrent()->getY()][(int)cameraQueue->getCurrent()->getX()]);
				 hazMap->getHazardMap()[(int)roverObject->getY()][(int)roverObject->getX()]=ROVER;
				 //Setting Information Boxes
				 xbox->set_int_val((int)roverObject->getModelX());
				 ybox->set_int_val((int)roverObject->getModelY());
				 zbox->set_int_val((int)roverObject->getZ());
				 headingbox->set_int_val((int)roverObject->getHeading());
				 //End of - 
				 hbox->set_int_val((int)roverMap->getMap()[(int)((roverObject->getX()*hazMap->getWidth())+roverObject->getY())]);
				 viewfinder();
				 hazMap->getHazardMap()[(int)roverObject->getY()][(int)roverObject->getX()] = PATH;
				 roverMap2->setHazardMap(roverMap->getHazardMap());				 
				 roverMap2->updateHazards();
				 viewCone();
				 //create a new path
				newPath = new astar((int)roverObject->getX(), (int)roverObject->getY(), cameraQueue->getLast()->getX(), cameraQueue->getLast()->getY(), roverMap);
				cameraQueue = newPath->pathQueue();
				}
				if(ATESTING == true)
				{
					//If in testing mode
					if((int)roverMap->getMap()[(int)((roverObject->getX()*hazMap->getWidth())+roverObject->getY())]==9)
					{
						//If rover is on a hazard level 9 then increment counter
						errorCount++;
					}
				}
		  
	}
	glutPostRedisplay();
	Sleep(100);
}




void glui_callback(int callback)
{
	//Call back events from GLUI controls
	switch (callback) {
	  case SETWAYPOINT:	if (pathWayQueue->getCurrent()!=NULL)
			 {
				 if(( WAYPOINT==false) && (pathWayQueue->getCurrent()->getNext()!=NULL))
				 {
				 WAYPOINT = true;
				 s = "FINISHED WAYPOINTS SETTING - press Start/Stop to start the viewer";
				 startStop->enable();
				 finishWayPoint->disable();
				 pathObject = new pathList(pathWayQueue, roverMap);	
				theactualpath = pathObject->theWay();
				cameraQueue = theactualpath->getCurrent()->getData();
				roverObject = new rover(hazMap->getWidth(), hazMap->getHeight(), cameraQueue->getCurrent()->getX(),cameraQueue->getCurrent()->getY(), (demObject->getHeightMap()[(int)cameraQueue->getCurrent()->getX()][(int)cameraQueue->getCurrent()->getY()]), 0, 10, 10);
				glutPostRedisplay();
						}
			 }
						break;
	  case STARTSTOP: 
		  if (WAYPOINT == true)
		  {
		  if ( START==false)
			 {
				 START = true;
				 s = "Viewing Started - press Start/Stop to Pause";
				 glutPostRedisplay();
			 }else if ( START == true)
			 {
				 START = false;
				 s = "Viewing Stopped - press Start/Stop to Continue";
				 glutPostRedisplay();
			 }
		  }else{
			  s = "Set waypoint(s) before starting, press waypoint button to finish setting waypoints";
			  glutPostRedisplay();
		  }
		  break;
	  case QUIT: exit(0);
		  break;
	  case RESET: //Reset everything
		  setupParts();
		  FINISHED = false;
			START = false;
			WAYPOINT = false;
			ATESTING = false;
			startStop->disable();
				 finishWayPoint->enable();
			s = "Program Reset - Set Waypoints and press waypoint button when finished";
		  break;
	  case VIEWING: if ( simple==false)
		 {
			 simple = true;
		 }else if ( simple == true)
		 {
			 simple = false;
		 }
		 break;
	  case AUTOMATICTESTING: if ((WAYPOINT == false)&&(START == false))
							 {
								 autoTest();
								 s = "AUTOMATIC TESTING IN PROGRESS";
								 startStop->disable();
				 finishWayPoint->disable();
								 glutPostRedisplay();
							 }else
							 {
								setupParts();
								  FINISHED = false;
									START = false;
									WAYPOINT = false;
									ATESTING = false;
									autoTest();
									startStop->disable();
				 finishWayPoint->disable();
								s = "AUTOMATIC TESTING IN PROGRESS";
								}
		  break;
			 
	}
}

void setupParts()
{
	//Initial Objects
	//Delete old objects
	cout << "Setting up environment. Please Wait.." << endl;
	delete demObject;
	delete hazMap;
	delete hazMap2;
	delete roverMap;
	delete roverMap2;
	delete pathWayQueue;
	delete roverObject;
	//Create image reader
	cout << "Create image reader..";
	imageReader = new ppmreader();
	cout << "done!" << endl;
	//create an image to hold the dem ppm
	cout << "Read in image..";
	imageObjectDem = imageReader->getImage("test.ppm");
	cout << "done!" << endl;
	//create a DEM object from the ppm
	cout << "Create DEM from Image..";
	demObject = new DEM(imageObjectDem);
	cout << "done!" << endl;
	//No need for ppm or image reader so free up memory
	cout << "Delete reader..";
	delete imageObjectDem;
	delete imageReader;
	cout << "done!" << endl;
	//create the hazardmaps from the DEM
	cout << "Create hazard maps from Image..";
	hazMap = new hazardMap(demObject);
	hazMap2 = new hazardMap(demObject);
	cout << "done!" << endl;
	//Create a queue object
	cout << "Setting up queue..";
	pathWayQueue = new queue();
	cout << "done!" << endl;
	//Create the hazard map for the rover, these start off empty
	cout << "Setting up rover..";
	roverMap = new hazardMap(demObject->getWidth(), demObject->getHeight());
	roverMap2 = new hazardMap(demObject->getWidth(), demObject->getHeight());
	//Setting Rover
	roverObject = new rover(hazMap->getWidth(), hazMap->getHeight(), 0, 0, 0.5, 0, 10, 10);
	knownMap = doubleArrayFloat(demObject->getWidth(), demObject->getHeight());
	for(int x = 0; x <= demObject->getHeight() - 1; x++)
	{
		for(int y = 0; y <= demObject->getWidth() - 1; y++)
		{
			knownMap[x][y] = 0;
		}
	}
	cout << "done!" << endl;
	cout << "Environment Created." << endl;
}

int
main(int argc, char *argv[])
{
	//int result = 0;
	char *what = "Based on PANGU Protocol Library Tester";
	char *who = "Martin N Dunstan";
	cout << "STL A* Search implementation\n(C)2001 Justin Heyes-Jones\n";
	cout << "Dynamic PathPlanning and PANGU Viewing(removed in this version) by Erin Maguire" << endl;
	srand((unsigned)time(0)); 
	/* Introduce ourselves */
	(void)fputs(what, stderr);
	(void)fputs(" by ", stderr);
	(void)fputs(who, stderr);
	(void)fputs("\n", stderr);

	/*Start all the parts needed to get it working*/
	setupParts();

	//Start glut window generation and GLUI controls creation
glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (1250, 700);
   glutInitWindowPosition (0, 0);
   //Creation and Setting of GLUT and GLUI components
   main_window = glutCreateWindow( "Display from Pathfinder" );
   //Creating GLUI windows
   glui_v_subwindow = GLUI_Master.create_glui_subwindow(main_window, GLUI_SUBWINDOW_RIGHT);
   glui_h_subwindow = GLUI_Master.create_glui_subwindow(main_window, GLUI_SUBWINDOW_BOTTOM);
glui_v_subwindow->set_main_gfx_window( main_window );
glui_h_subwindow->set_main_gfx_window( main_window );
//Controls panel
GLUI_Panel *op_panel = glui_v_subwindow->add_panel ("Controls");
GLUI_Panel *zp_panel = glui_v_subwindow->add_panel ("Hazard Levels(key)");
//adding controls
finishWayPoint = glui_v_subwindow->add_button_to_panel(op_panel, "Finish Waypoint Setting", SETWAYPOINT, glui_callback);
startStop = glui_v_subwindow->add_button_to_panel(op_panel, "START/STOP", STARTSTOP, glui_callback);
startStop->disable();
glui_v_subwindow->add_button_to_panel(op_panel, "Field of View", VIEWING, glui_callback);

glui_v_subwindow->add_button_to_panel(op_panel, "Reset", RESET, glui_callback);
glui_v_subwindow->add_button_to_panel(op_panel, "Quit", QUIT, glui_callback);
glui_h_subwindow->add_statictext_to_panel(zp_panel, "Hardest to Easiest");
glui_h_subwindow->add_statictext_to_panel(zp_panel, "9 - Red - Unpassable");
glui_h_subwindow->add_statictext_to_panel(zp_panel, "5 - Yellow - Hard");
glui_h_subwindow->add_statictext_to_panel(zp_panel, "4 -  - Hard/Medium");
glui_h_subwindow->add_statictext_to_panel(zp_panel, "3 -  - Medium");
glui_h_subwindow->add_statictext_to_panel(zp_panel, "2 -  - Easy");
glui_h_subwindow->add_statictext_to_panel(zp_panel, "1 - Grey - Passable");
glui_v_subwindow->add_button_to_panel(op_panel, "Auto Test", AUTOMATICTESTING, glui_callback);

//Information Panel
GLUI_Panel *bp_panel = glui_h_subwindow->add_panel ("Rover Information");

//Adding in information links
int xvalue = 55;
xbox = glui_h_subwindow->add_edittext_to_panel(bp_panel, "X", xvalue, 0, -1, glui_callback);
glui_h_subwindow->add_column_to_panel(bp_panel, true);
ybox = glui_h_subwindow->add_edittext_to_panel(bp_panel, "Y", xvalue, 0, -1, glui_callback);
glui_h_subwindow->add_column_to_panel(bp_panel, true);
zbox = glui_h_subwindow->add_edittext_to_panel(bp_panel, "Z", xvalue, 0, -1, glui_callback);
glui_h_subwindow->add_column_to_panel(bp_panel, true);
hbox = glui_h_subwindow->add_edittext_to_panel(bp_panel, "Hazard Level", xvalue, 0, -1, glui_callback);
glui_h_subwindow->add_column_to_panel(bp_panel, true);
headingbox = glui_h_subwindow->add_edittext_to_panel(bp_panel, "Heading", xvalue, 0, -1, glui_callback);
glui_h_subwindow->add_column_to_panel(bp_panel, false);
glui_h_subwindow->add_statictext_to_panel(bp_panel, "degrees");
//end of - glui controls

   s = "Set waypoints then press waypoint button to finish";
	init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouse);
   GLUI_Master.set_glutIdleFunc( idle );
   glutMainLoop();

	return 0;
}

