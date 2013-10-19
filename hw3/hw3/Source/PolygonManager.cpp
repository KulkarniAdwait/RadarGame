#include "../Headers/polygonManager.h"

//Add a new polygon to the GPU buffer
int PolygonManager::AddPolygon(int vertexCount, std::vector<GLfloat> vertices, std::vector<GLfloat> colors, MyGraphicsDevice& gDevice)
{
	GLintptr lVertices = -1, lColors = -1;
	int lCount = -1;
	gDevice.PushData(vertexCount, vertices, colors, &lVertices, &lColors, &lCount);
	PolygonData poly = PolygonData(vertexCount, lVertices, vertices, lColors, colors, lCount);
	polygons.push_back(poly);
	//return &poly;
	return polygons.size() - 1;
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