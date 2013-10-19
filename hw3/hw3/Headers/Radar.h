#ifndef RADAR_H
#define RADAR_H
#include "Header.h"
#include "GraphicsDevice.h"
#include "polygonData.h"
#include "polygonManager.h"
#include "GameConstants.h"
#include <vector>

class Radar
{
private:
	std::vector<GLfloat> radarVertices;
	std::vector<GLfloat> radarColors;
	//polygonData* poly;
	int polyIndex;
	float angle;
	
public:
	glm::vec2 radarVec;
	float origX, origY;
	float radius;
	Radar()
	{
		//poly = NULL;
		polyIndex = -1;
		angle = -3.0f;
		radius = SCREEN_WIDTH / 2;
		//origin
		radarVertices.push_back(SCREEN_WIDTH / 2);
		radarVertices.push_back(SCREEN_HEIGHT / 2 - SHIFT_DUE_TO_UI);
		radarVertices.push_back(0.0f);
		radarVertices.push_back(1.0f);

		radarVertices.push_back((float)SCREEN_WIDTH / 2.0f);
		radarVertices.push_back(0.0f);
		radarVertices.push_back(0.0f);
		radarVertices.push_back(1.0f);

		radarColors.push_back(0.0f);
		radarColors.push_back(1.0f);
		radarColors.push_back(0.0f);
		radarColors.push_back(1.0f);

		radarColors.push_back(0.0f);
		radarColors.push_back(1.0f);
		radarColors.push_back(0.0f);
		radarColors.push_back(1.0f);

		origX = SCREEN_WIDTH / 2;
		origY = SCREEN_HEIGHT / 2 - SHIFT_DUE_TO_UI;

		radarVec = glm::vec2(0.0f, 0.0f);
	}

	void PushData(PolygonManager& polygons, MyGraphicsDevice& gDevice)
	{
		//poly = polygons.addPolygon(2, radarVertices, radarColors, gDevice);
		polyIndex = polygons.AddPolygon(2, radarVertices, radarColors, gDevice);
		radarVertices.clear();
		radarColors.clear();
	}

	void Update(PolygonManager& polygons)
	{
		if(polyIndex != -1)
		{
			polygons.RotatePolygon(0, polyIndex, angle);
			radarVec.x = polygons.polygons[polyIndex].vertexData[4] - polygons.polygons[polyIndex].vertexData[0];
			radarVec.y = polygons.polygons[polyIndex].vertexData[5] - polygons.polygons[polyIndex].vertexData[1];
		}
	}
};

#endif