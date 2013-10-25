#ifndef EXPLOSION_H
#define EXPLOSION_H
#define COLOR_TIME 10
#include "Header.h"
#include "GraphicsDevice.h"
#include "polygonData.h"
#include "polygonManager.h"
#include "GameConstants.h"
#include "Radar.h"
#include <vector>

class Explosion
{
private:
	//this is the index into the polygonManager
	int polyIndex;
	//this is the time that the object will be alive
	//counts down
	int timer;

	//internal flag for color switching
	bool centerIsRed;
	//this is the time since last color switch
	//used for animation
	int colorTimer;
	//This store the origin of the object
	//All calculations must be carried out w.r.t these co ordinates
	GLfloat originX, originY;

	//radius of the objects in object space
	static const int RADIUS = 15;
	//this angle decides the number of line segments used to 
	//approximate the outer circle. 12 = 360/30
	static const int ANGLE = 30;

	void RotatePoints(float x, float y, float& rotX, float& rotY, int angle);
public:
	bool isAlive;

	int getPolygonIndex() { return this->polyIndex; }

	Explosion(GLfloat startX, GLfloat startY, PolygonManager& polygons, MyGraphicsDevice& gDevice)
	{
		timer = EXPLOSION_TIME;
		colorTimer = COLOR_TIME;
		isAlive = true;
		originX = startX;
		originY = startY;
		polyIndex = -1;
		PushData(polygons, gDevice);
	}

	void PushData(PolygonManager& polygons, MyGraphicsDevice& gDevice);

	void Update(PolygonManager& polygons, MyGraphicsDevice& gDevice);

};

#endif