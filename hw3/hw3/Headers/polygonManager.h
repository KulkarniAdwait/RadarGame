#ifndef MY_POLYGON_DATA_MANAGER_H
#define MY_POLYGON_DATA_MANAGER_H

#define VERTICES 0
#define COLORS 1
#define INDICES 2
#include "Header.h"
#include "polygonData.h"
#include "freeSpots.h"
#include "GraphicsDevice.h"
#include <vector>
#include <list>

using namespace std;
//Class that holds the actual list of polygons in the GPU
class PolygonManager
{
private:
	//std::list<polygonData> polygons;
	std::list<freeSpot> deletedPolygons;

public:
	std::vector<PolygonData> polygons;

	PolygonManager()
	{
		polygons.clear();
		deletedPolygons.clear();
	}

	~PolygonManager()
	{
		polygons.clear();
		deletedPolygons.clear();
	}

	//Add a new polygon to the GPU buffer
	//polygonData* addPolygon(int vertexCount, std::vector<GLfloat> vertices, std::vector<GLfloat> colors, MyGraphicsDevice& gDevice)
	int AddPolygon(int vertexCount, std::vector<GLfloat> vertices, std::vector<GLfloat> colors, MyGraphicsDevice& gDevice);

	//translates the given polygon to the dest X & Y
	//the selectedIndex is used as the origin of the translation
	void TranslatePolygon(int destX, int destY, int polygonIndex, int selectedIndex);

	//rotates the given polygon around the selected index
	//the selectedIndex is used as the origin of the translation
	void RotatePolygon(int selectedIndex, int polygonIndex, float angle);
	
	void Display(MyGraphicsDevice gDevice)
	{

	}

	void Update(MyGraphicsDevice gDevice)
	{
		//only push data for those polygons that have been updated
		for( int i = 0; i < polygons.size() ; i++ )
		{
			if(polygons[i].updated == true)
			{
				gDevice.UpdateData(polygons[i]);
				polygons[i].updated == false;
			}
		}
	}

	//deletes the given polygon from the GPU buffer
	//makes entry into deleted list
	void deletePolygon(int polygonIndex, MyGraphicsDevice gDevice);

	void ChangeColor(int polygonIndex, float newRed, float newGreen, float newBlue)
	{
		for( unsigned int i = 0; i < polygons[polygonIndex].vertexColors.size(); i+=4 )
		{
			polygons[polygonIndex].vertexColors[i] = newRed;
			polygons[polygonIndex].vertexColors[i + 1] = newGreen;
			polygons[polygonIndex].vertexColors[i + 2] = newBlue;			
		}
		polygons[polygonIndex].updated = true;
	}
};

#endif