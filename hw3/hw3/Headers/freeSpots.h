#ifndef FREE_SPOTS_H
#define FREE_SPOTS_H

#include <vector>

//holds the location and the number of vertices a slot can hold
class freeSpot
{
public:
	int vertexCount;
	int startPosition;

	freeSpot()
	{
		this->vertexCount = -1;
		this->startPosition = -1;
	}

	freeSpot(int vertexCount, int startPosition)
	{
		this->vertexCount = vertexCount;
		this->startPosition = startPosition;
	}
};

#endif