#include "../Headers/FlyingObject.h"
#include <time.h>
#include <stdlib.h>

#define DIVISIONS 10
enum Sides {Top, Left, Right, Bottom };
//discretize the walls of the world and then select a start and end position for the flying objects at random
float Y_VALS[DIVISIONS] = {0.0f //top
	, (SCREEN_HEIGHT - SHIFT_DUE_TO_UI) / 9.0f
	, 2 * (SCREEN_HEIGHT - SHIFT_DUE_TO_UI) / 9.0f
	, (SCREEN_HEIGHT - SHIFT_DUE_TO_UI) //bottom
	, 3 * (SCREEN_HEIGHT - SHIFT_DUE_TO_UI) / 9.0f
	, 4 * (SCREEN_HEIGHT - SHIFT_DUE_TO_UI) / 9.0f
	, 5 * (SCREEN_HEIGHT - SHIFT_DUE_TO_UI) / 9.0f
	, 6 * (SCREEN_HEIGHT - SHIFT_DUE_TO_UI) / 9.0f
	, 7 * (SCREEN_HEIGHT - SHIFT_DUE_TO_UI) / 9.0f
	, 8 * (SCREEN_HEIGHT - SHIFT_DUE_TO_UI) / 9.0f
	};
float X_VALS[DIVISIONS] = {SCREEN_WIDTH / 9.0f
	, 0.0f //left
	, SCREEN_WIDTH //right
	, 2 * SCREEN_WIDTH / 9.0f
	, 3 * SCREEN_WIDTH / 9.0f
	, 4 * SCREEN_WIDTH / 9.0f
	, 5 * SCREEN_WIDTH / 9.0f
	, 6 * SCREEN_WIDTH / 9.0f
	, 7 * SCREEN_WIDTH / 9.0f
	, 8 * SCREEN_WIDTH / 9.0f
	};

float VectorDot(glm::vec2 v1, glm::vec2 v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y);
}

float Magnitude(glm::vec2 v)
{
	return glm::sqrt((v.x * v.x) + (v.y * v.y));
}

float DistanceFormula (float x1, float x2, float y1, float y2)
{
	return glm::sqrt(((x2- x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1)));
}

FlyingObject::FlyingObject(int startSide, int difficulty, PolygonManager& polygons, MyGraphicsDevice& gDevice)
{
	recognized = false;
	endReached = false;
	SPEED = difficulty * 0.0005f;

	polyIndex = -1;
	alpha = 0.0f;

	//1 in 8 chance of being friendly
	if( (std::rand() % 8) == 0 )
	{
		isFriendly = true;
	}
	else
	{
		isFriendly = false;
	}
	
	switch(startSide)
	{
		//TOP
		case Top:
			//select minY as startY and maxY as bottom
			startY = Y_VALS[Top];
			dirY = Y_VALS[Bottom];
			//start and end Y can be random
			startX = X_VALS[std::rand() % DIVISIONS];
			dirX = X_VALS[std::rand() % DIVISIONS] - startX;
		break;
		//LEFT
		case Left:
			//select left as startX and right as endX
			startX = X_VALS[Left];
			dirX = X_VALS[Right];
			//start and end Y can be random
			startY = Y_VALS[std::rand() % DIVISIONS];
			dirY = Y_VALS[std::rand() % DIVISIONS] - startY;
		break;
		//BOTTOM
		case Bottom:
			//select maxY as startY and minY as bottom
			startY = Y_VALS[Bottom];
			dirY = Y_VALS[Top] - startY;
			//start and end Y can be random
			startX = X_VALS[std::rand() % DIVISIONS];
			dirX = X_VALS[std::rand() % DIVISIONS] - startX;
		break;
		//RIGHT
		case Right:
			//select right as startX and left as endX
			startX = X_VALS[Right];
			dirX = X_VALS[Left] - startX;
			//start and end Y can be random
			startY = Y_VALS[std::rand() % DIVISIONS];
			dirY = Y_VALS[std::rand() % DIVISIONS] - startY;
		break;

	}
	originX = 0.0f;
	originY = 0.0f;

	objVec = glm::vec2(0.0f, 0.0f);
	PushData(polygons, gDevice);
}

void RotatePoints(float x, float y, float& rotX, float& rotY, int angle)
{
	using namespace glm;
	float aRadians = radians((float)angle);
		
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

void FlyingObject::PushData(PolygonManager& polygons, MyGraphicsDevice& gDevice)
{
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> colors;
	int numSides = 360 / ANGLE;
	float oldX = 0.0f;
	float oldY = (float)RADIUS;
	vertices.push_back(oldX);
	vertices.push_back(oldY);
	vertices.push_back(0.0f);
	vertices.push_back(1.0f);

	//find the next n sides by rotating by objAngle
	//about the origin 0,0
	for(int i = 0; i < numSides; i++ )
	{
		RotatePoints(oldX, oldY, oldX, oldY, ANGLE);
		vertices.push_back(oldX);
		vertices.push_back(oldY);
		vertices.push_back(0.0f);
		vertices.push_back(1.0f);

		colors.push_back(1.0f);
		colors.push_back(1.0f);
		colors.push_back(1.0f);
		colors.push_back(1.0f);
	}

	polyIndex = polygons.AddPolygon(numSides, vertices, colors, gDevice);
	vertices.clear();
	colors.clear();
}

void FlyingObject::Update(PolygonManager& polygons, Radar* radar)
{
	if(polyIndex != -1 && alpha <= 1.0f)
	{
		//		P = Start + alpha * DirectionVector
		originX = startX + alpha * dirX;
		originY = startY + alpha * dirY;
		polygons.TranslatePolygon(originX, originY, polyIndex, 0);
		alpha += SPEED;
	}

	//if radar and obj are within a certain angle AND object is within distance of radar
	//then change color to green/red depending on type
	objVec.x = originX - radar->origX;
	objVec.y = originY - radar->origY;
	if(!recognized 
		//if angle between the radar and the object w.r.t radar center is less than the offset
		&& glm::acos((VectorDot(objVec, radar->radarVec) / (Magnitude(objVec) * Magnitude(radar->radarVec)))) <= (glm::radians((float)ANGLE_OFFSET))
		&& DistanceFormula(originX, radar->origX, originY, radar->origY) <= (radar->radius + RADIUS + RADIUS))
	{
		//Green color
		if(isFriendly)
		{
			polygons.ChangeColor(polyIndex, 0.0f, 1.0f, 0.0f);
		}
		else
		{
			polygons.ChangeColor(polyIndex, 1.0f, 0.0f, 0.0f);
		}
		recognized = true;
	}

	//if object has reached the end of it's path
	if(alpha > 1.0f)
		endReached = true;
}

bool FlyingObject::CheckHit(int x, int y, PolygonManager& polygons, MyGraphicsDevice& gDevice)
{
	if( DistanceFormula(originX, x, originY, y) <= 2 * RADIUS )
	{
		polygons.deletePolygon(this->polyIndex, gDevice);
		return true;
	}
	return false;
}