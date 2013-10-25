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
	static const int TRACE_VERT_SIZE = 360;
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
	
	void PushData(PolygonManager& polygons, MyGraphicsDevice& gDevice);
public:
	glm::vec2 radarVec;
	float origX, origY;
	float radius;

	Radar(PolygonManager& polygons, MyGraphicsDevice& gDevice);		

	void Update(PolygonManager& polygons);

};

#endif