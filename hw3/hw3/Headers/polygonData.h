#ifndef MY_POLYGON_DATA_H
#define MY_POLYGON_DATA_H

#include "Header.h"
#include <vector>

//holds all the data about a polygon/primitive
class PolygonData
{
public:
	//has this polygon been updated
	bool updated;
	//location in the counts array for glMultiDraw
	int countLocation;
	//number of vertices in the polygon
	int vertexCount;
	//location in bytes of the polygon in GPU buffer
	GLintptr location;
	//location in bytes of the color in the GPU buffer
	GLintptr colorLocation;
	//vertex data in world co-ordinates
	std::vector<GLfloat> vertexData;
	//color data
	std::vector<GLfloat> vertexColors;

	PolygonData()
	{
		this->updated = false;
		this->countLocation = 0;
		this->vertexCount = 0;
		this->location = 0;
		this->colorLocation = 0;
		this->vertexData.clear();
		this->vertexColors.clear();
	}

	PolygonData(int vertexCount, GLintptr location, std::vector<GLfloat> vertices, GLintptr colorLocation, std::vector<GLfloat> colors, int countLocation)
	{
		this->updated = false;
		this->countLocation = countLocation;
		this->vertexCount = vertexCount;
		this->location = location;
		this->colorLocation = colorLocation;
		this->vertexData = vertices;
		this->vertexColors = colors;
	}
};

#endif