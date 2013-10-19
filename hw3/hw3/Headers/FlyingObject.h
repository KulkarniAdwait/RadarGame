#ifndef FLYING_OBJECTS_H
#define FLYING_OBJECTS_H
#include "Header.h"
#include "GraphicsDevice.h"
#include "polygonData.h"
#include "polygonManager.h"
#include "GameConstants.h"
#include "Radar.h"
#include <vector>

class FlyingObject
{
private:
	bool isFriendly;
	bool recognized;
	//this is the index into the polygonManager
	int polyIndex;

	//This store the start points
	GLfloat startX, startY;

	//This store the origin of the object
	//All calculations must be carried out w.r.t these co ordinates
	GLfloat originX, originY;

	//Parametric line equation variables
	GLfloat alpha;
	GLfloat dirX, dirY;

	//speed of the object
	GLfloat SPEED;
	//radius of the objects in object space
	static const int RADIUS = 15;
	//this angle decides the number of line segments used to 
	//approximate the outer circle. 12 = 360/30
	static const int ANGLE = 30;
	//this angle is the offset that decides if the object is in range of the radar
	static const int ANGLE_OFFSET = 5;

	//vector betn object and origin
	glm::vec2 objVec;

	void RotatePoints(float x, float y, float& rotX, float& rotY)
	{
		using namespace glm;
		float aRadians = radians((float)ANGLE);
		
		//rotation matrix in column major
		mat4x4 Rmat = mat4x4(cos(aRadians), -sin(aRadians), 0.0f, 0.0f,
						sin(aRadians), cos(aRadians), 0.0f, 0.0f, 
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f);

		vec4 Overts, Rverts;
		
		Overts = vec4(x, y, 0.f, 1.f);
		Rverts = Rmat * Overts;
		rotX = Rverts.x;
		rotY = Rverts.y;
		//not updating z & w because they are always 0.0 & 1.0		
	}
public:
	FlyingObject(int);

	void PushData(PolygonManager& polygons, MyGraphicsDevice& gDevice);

	void Update(PolygonManager& polygons, Radar* radar);

	void CheckHit(int x, int y, PolygonManager& polygons, MyGraphicsDevice& gDevice);

};

#endif