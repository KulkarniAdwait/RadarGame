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

	int polyIndex;
	float angle;

	//hold the outer circumference of the radar
	std::vector<GLfloat> traceVertices;
	std::vector<GLfloat> traceColors;

	void RotatePoints(float x, float y, float& rotX, float& rotY, int angle)
	{
		using namespace glm;
		int destX = SCREEN_WIDTH / 2;
		int destY = (SCREEN_HEIGHT / 2 - SHIFT_DUE_TO_UI);
		float aRadians = radians((float)angle);
		
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
		
		Overts = vec4(x, y, 0.f, 1.f);
		Rverts = Rmat * Overts;
		rotX = Rverts.x;
		rotY = Rverts.y;
		//not updating z & w because they are always 0.0 & 1.0		
	}
	
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
		radarVertices.push_back(SCREEN_WIDTH / 2.0f);
		radarVertices.push_back(SCREEN_HEIGHT / 2.0f - SHIFT_DUE_TO_UI);
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

		//trace of the circumference of the radar
		float oldX = 0.0f;
		float oldY = (SCREEN_HEIGHT / 2.0f - SHIFT_DUE_TO_UI);
		traceVertices.push_back(oldX);
		traceVertices.push_back(oldY);
		traceVertices.push_back(0.0f);
		traceVertices.push_back(1.0f);

		traceColors.push_back(0.05f);
		traceColors.push_back(1.0f);
		traceColors.push_back(0.05f);
		traceColors.push_back(0.1f);

		for( int i = 0; i < 179; i++ )
		{
			RotatePoints(oldX, oldY, oldX, oldY, 2);

			traceVertices.push_back(oldX);
			traceVertices.push_back(oldY);
			traceVertices.push_back(0.0f);
			traceVertices.push_back(1.0f);
	
			traceColors.push_back(0.0f);
			traceColors.push_back(1.0f);
			traceColors.push_back(0.5f);
			traceColors.push_back(0.1f);
		}
	}

	void PushData(PolygonManager& polygons, MyGraphicsDevice& gDevice)
	{
		polyIndex = polygons.AddPolygon(2, radarVertices, radarColors, gDevice);
		radarVertices.clear();
		radarColors.clear();

		polygons.AddPolygon(180, traceVertices, traceColors, gDevice);
		traceVertices.clear();
		traceColors.clear();
	}

	void Update(PolygonManager& polygons)
	{
		if(polyIndex != -1)
		{
			polygons.RotatePolygon(0, polyIndex, angle);
			//x is at 0 and 4
			radarVec.x = polygons.polygons[polyIndex].vertexData[4] - polygons.polygons[polyIndex].vertexData[0];
			//y is at 1 and 5
			radarVec.y = polygons.polygons[polyIndex].vertexData[5] - polygons.polygons[polyIndex].vertexData[1];
		}
	}
};

#endif