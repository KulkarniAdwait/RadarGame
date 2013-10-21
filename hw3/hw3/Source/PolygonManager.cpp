#include "../Headers/polygonManager.h"

//Add a new polygon to the GPU buffer
int PolygonManager::AddPolygon(int vertexCount, std::vector<GLfloat> vertices, std::vector<GLfloat> colors, MyGraphicsDevice& gDevice)
{
	if( deletedPolygons.size() > 0 )
	{
		int pIndex = deletedPolygons.front().polyIndex;
		polygons[pIndex].countLocation = deletedPolygons.front().countLocation;
		polygons[pIndex].location = deletedPolygons.front().startPosition;
		polygons[pIndex].colorLocation = deletedPolygons.front().colorLocation;
		polygons[pIndex].updated = true;
		polygons[pIndex].vertexCount = deletedPolygons.front().vertexCount;
		polygons[pIndex].vertexData = vertices;
		polygons[pIndex].vertexColors = colors;

		gDevice.UpdateData(polygons[pIndex]);

		deletedPolygons.pop_front();

		return pIndex;
	}
	else
	{
		GLintptr lVertices = -1, lColors = -1;
		int lCount = -1;
		gDevice.PushData(vertexCount, vertices, colors, &lVertices, &lColors, &lCount);
		PolygonData poly = PolygonData(vertexCount, lVertices, vertices, lColors, colors, lCount);
		polygons.push_back(poly);
		return polygons.size() - 1;
	}
}

//deletes polygon from polygon list
void PolygonManager::deletePolygon(int polygonIndex, MyGraphicsDevice gDevice)
{
	for( unsigned int i = 0; i < polygons[polygonIndex].vertexData.size() ; i+=4 )
	{
		polygons[polygonIndex].vertexData[i] = 0.0f;
		polygons[polygonIndex].vertexData[i + 1] = 0.0f;
		polygons[polygonIndex].vertexData[i + 2] = 0.0;
		polygons[polygonIndex].vertexData[i + 3] = 0.0;
	}
	
	for( unsigned int i = 0; i < polygons[polygonIndex].vertexColors.size() ; i+=4 )
	{
		polygons[polygonIndex].vertexColors[i] = 0.0f;
		polygons[polygonIndex].vertexColors[i + 1] = 0.0f;
		polygons[polygonIndex].vertexColors[i + 2] = 0.0;
		polygons[polygonIndex].vertexColors[i + 3] = 0.0;
	}

	//add metadata about deleted polygon to deleted list
	freeSpot freed = freeSpot(polygons[polygonIndex].vertexCount, polygons[polygonIndex].location, polygons[polygonIndex].colorLocation
			, polygons[polygonIndex].countLocation, polygonIndex);
	deletedPolygons.push_back(freed);
	gDevice.UpdateData(polygons[polygonIndex]);
	gDevice.DeletePolygon(polygons[polygonIndex].countLocation);
}


//translates the given polygon to the dest X & Y
//the selectedIndex is used as the origin of the translation
void PolygonManager::TranslatePolygon(int destX, int destY, int polygonIndex, int selectedIndex)
{
	using namespace glm;
	float chX = destX - polygons[polygonIndex].vertexData[selectedIndex];
	float chY = destY - polygons[polygonIndex].vertexData[selectedIndex + 1];
		
	//translation matrix in row major
	mat4x4 Tmat = mat4x4(1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f, 
					0.0f, 0.0f, 1.0f, 0.0f,
					chX, chY, 0.0f, 1.0f);
	vec4 Overts, Tverts;
	for( unsigned int i = 0; i < polygons[polygonIndex].vertexData.size(); i+=4 )
	{
		Overts = vec4(polygons[polygonIndex].vertexData[i], polygons[polygonIndex].vertexData[i + 1], 0.f, 1.f);
		Tverts = Tmat * Overts;
		polygons[polygonIndex].vertexData[i] = Tverts.x;
		polygons[polygonIndex].vertexData[i + 1] = Tverts.y;
		//not updating z & w because they are always 0.0 & 1.0
	}
	polygons[polygonIndex].updated = true;
}

//rotates the given polygon around the selected index
//the selectedIndex is used as the origin of the translation
//void rotatePolygon(int selectedIndex, polygonData& polygon, float angle)
void PolygonManager::RotatePolygon(int selectedIndex, int polygonIndex, float angle)
{
	using namespace glm;
	int destX = polygons[polygonIndex].vertexData[selectedIndex];
	int destY = polygons[polygonIndex].vertexData[selectedIndex + 1];
	float aRadians = radians(angle);
		
	//translate back to original
	mat4x4 Tmat = mat4x4(1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f, 
					0.0f, 0.0f, 1.0f, 0.0f,
					destX, destY, 0.0f, 1.0f);

	//rotation matrix in column major
	mat4x4 RotMat = mat4x4(cos(aRadians), -sin(aRadians), 0.0f, 0.0f,
					sin(aRadians), cos(aRadians), 0.0f, 0.0f, 
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);

	//translate to the origin
	mat4x4 Bmat = mat4x4(1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f, 
					0.0f, 0.0f, 1.0f, 0.0f,
					-destX, -destY, 0.0f, 1.0f);
		
	mat4x4 Rmat = Tmat * RotMat * Bmat;

	vec4 Overts, Rverts;
	for( unsigned int i = 0; i < polygons[polygonIndex].vertexData.size(); i+=4 )
	{
		Overts = vec4(polygons[polygonIndex].vertexData[i], polygons[polygonIndex].vertexData[i + 1], 0.f, 1.f);
		Rverts = Rmat * Overts;
		polygons[polygonIndex].vertexData[i] = Rverts.x;
		polygons[polygonIndex].vertexData[i + 1] = Rverts.y;
		//not updating z & w because they are always 0.0 & 1.0
	}
	polygons[polygonIndex].updated = true;
}

////x and y must be in world co ordinates
////returns the index of the vertex of the selected polygon
////i.e. which vertex did the user click
//int PolygonManager::FindPolygon(const int x, const int y, const int SELECTION_OFFSET)
//{
//	std::vector<PolygonData>::iterator it;
//	int index = 0;
//	for(it = polygons.begin(); it != polygons.end(); it++)
//	{
//		for(unsigned int i=0; i < (*it).vertexData.size(); i+=4)
//		{
//			if(x + SELECTION_OFFSET >= (*it).vertexData[i] && x - SELECTION_OFFSET <= (*it).vertexData[i]
//				&& y + SELECTION_OFFSET >= (*it).vertexData[i+1] && y - SELECTION_OFFSET <= (*it).vertexData[i+1])
//			{
//				return index;
//			}
//		}
//		index++;
//	}
//	return -1;
//}