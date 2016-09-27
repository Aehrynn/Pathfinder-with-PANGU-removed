#include "ppmreader.h"

ppmreader::ppmreader()
{
//	memblock = "";
//	memblock2 = "";
	height = 0;
	width = 0;
	colour = 0;
	firstDot = 0;
	secondDot = 0;
	firstSpace = 0;
	secondSpace = 0;
	heightMap = 0;
	pointer = 0;
}
ppmreader::~ppmreader()
{
	//delete [] heightMap;
		for (int w = 0; w < width-1; w++)
		{
			for (int x = 0; x < height-1; x++)
		{
			delete [] heightMap[w][x];
		}
			delete [] heightMap[w];
		}
	delete [] heightMap;
	//delete imageObject;
}


void ppmreader::setData()
{
	//Assign every data element into the array, after every 3rd element change dimension
	for (int i = 0; i < height; i++)
		{
			for (int z =0; z < width; z++)
			{
				for (int x = 0; x < 3; x++)
				{
					heightMap[i][z][x] = float(int(unsigned char(memblock[pointer])));
					pointer++;
				}
				
			}
		}

		//Creating an object with which to store the image attributes
		imageObject = new ppm(float(width), float(height), float(colour), heightMap);
}

void ppmreader::setData2()
{
	//Assign every data element into the array, after every 3rd element change dimension
	for (int i = 0; i < height*width*3; i++)
		{
			
					imageMap[i] = memblock[pointer];
					pointer++;
			
		}

		//Creating an object with which to store the image attributes
		//imageObject = new ppm(float(width), float(height), float(colour), heightMap);
}
void ppmreader::setupArray()
{
	//setting up height and width values
	//Seems to store the proper values for the height and width as the chars but not as the correct hex value
	//eg 0-9 is actually 48-57, so taking 48 from the value gets us the correct integer value to use
	//Get the height value from the header
	int multiplier = 1;
	for ( int f = firstSpace - 1; f > firstDot; f--)
	{
		height += multiplier * int(memblock[f]-48);
		multiplier *= 10;
	}
	//Get the width value from the header
	multiplier = 1;
	for ( int f = secondSpace - 1; f > firstSpace; f--)
	{
		width += multiplier * int(memblock[f]-48);
		multiplier *= 10;
	}
	//Get the colour value from the header
	multiplier = 1;
	for ( int f = secondDot - 1; f > secondSpace; f--)
	{
		colour += multiplier * int(memblock[f]-48);
		multiplier *= 10;
	}
		//Set up the 3 dimensional array based on header information	 
		heightMap = tripleArrayFloat((float)height, (float)width, 3);
}

void ppmreader::retrieveHeader()
{
	//Finds the location in the array of the header information
	//P6.512 512 256. is the makeup of the header,  this function picks out the dots and spaces of the header
	//so that they can be used later
	bool found = false;
	
	//Find the first dot
	while (found == false)
	{
		
	if (int(memblock[pointer]) == 10)
		{
			found = true;
			firstDot = pointer;
			pointer++;
	}else{
		pointer++;
	}
	}
	//now get the first 'white space' which is integer 32 hex is 20 then extract the heigth between it, then find the second space and then find the width
	found = false;
	while (found == false)
	{
		
	if (int(memblock[pointer]) == 32)
		{
			
			found = true;
			firstSpace = pointer;
			pointer++;
	}else{
		pointer++;
	}
	}
	//Find the second white space
	found = false;
	while (found == false)
	{
		
	if (int(memblock[pointer]) == 32)
		{
			found = true;
			secondSpace = pointer;
			pointer++;
	}else{
		pointer++;
	}
	}
	//Find the last dot that which ends the header information
	found = false;
	while (found == false)
	{
		
	if (int(memblock[pointer]) == 10)
		{
			
			found = true;
			secondDot = pointer;
			pointer++;
	}else{
		pointer++;
	}
	}
}


ppm * ppmreader::convertImage(char *fileName) 
{
	//Take in a filename to convert into a PPM Object for return
	ifstream::pos_type size;
	//ifstream file ("test.ppm", ios::in|ios::binary|ios::ate);
	ifstream file (fileName, ios::in|ios::binary|ios::ate);
  if (file.is_open())
  {
    size = file.tellg();
    memblock2 = new char [size];	
    file.seekg (0, ios::beg);
    file.read (memblock2, size);	
    file.close();

	memblock = (unsigned char *)memblock2;

	retrieveHeader();
	setupArray();
	setData();
  }
  else cout << "Unable to open file";
  delete [] memblock;
//  delete [] memblock2;
 // delete [] heightMap;
  return imageObject;
}

ppm * ppmreader::convertImagePANGU(unsigned char * newArray) 
{
	//Take an array and convert into a PPM Object then return this object
    memblock = newArray;
	//cout << "Yarr!";
	retrieveHeader();
	setupArray();
	setData();
  delete [] memblock;
  return imageObject;
}

unsigned char * ppmreader::convertImagePANGU2(unsigned char * newArray) 
{
	//Take an array and convert into a PPM Object then return this object
    memblock = newArray;
	//cout << "Yarr!";
	retrieveHeader();
	setupArray();
	setData2();
  delete [] memblock;
  return imageMap;
}

ppm * ppmreader::getImage(char *fileName)
{
	//Take in a filename and return a ppm object
	return convertImage(fileName);
}

ppm * ppmreader::getPanguImage(unsigned char * newArray)
{
	//Take in a array and return a ppm object
	return convertImagePANGU(newArray);
}

unsigned char * ppmreader::getPanguImage2(unsigned char * newArray)
{
	//Take in a array and return a ppm object
	return convertImagePANGU2(newArray);
}