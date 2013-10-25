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

public:
	bool isFriendly;
	bool endReached;
	bool recognized;

	int getPolygonIndex() { return this->polyIndex; }
	GLfloat getOriginX() { return this->originX; }
	GLfloat getOriginY() { return this->originY; }
	FlyingObject(int startSide, int difficulty, PolygonManager& polygons, MyGraphicsDevice& gDevice);

	void PushData(PolygonManager& polygons, MyGraphicsDevice& gDevice);

	void Update(PolygonManager& polygons, Radar* radar);

	bool CheckHit(int x, int y, PolygonManager& polygons, MyGraphicsDevice& gDevice);

};

#endif