#ifndef FREE_SPOTS_H
#define FREE_SPOTS_H

#include <vector>

//holds the location and the number of vertices a slot can hold
class freeSpot
{
public:
	int vertexCount;
	int startPosition;
	int colorLocation;
	int countLocation;
	int polyIndex;

	freeSpot()
	{
		this->vertexCount = -1;
		this->startPosition = -1;
		this->countLocation = -1;
		this->polyIndex = -1;
		this->colorLocation = -1;
	}

	freeSpot(int vertexCount, int startPosition, int colorLocation, int countLocation, int polyIndex)
	{
		this->vertexCount = vertexCount;
		this->startPosition = startPosition;
		this->countLocation = countLocation;
		this->polyIndex = polyIndex;
		this->colorLocation = colorLocation;
	}
};

#endif