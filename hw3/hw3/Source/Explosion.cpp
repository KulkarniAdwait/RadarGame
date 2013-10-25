#include "../Headers/Explosion.h"

void Explosion::RotatePoints(float x, float y, float& rotX, float& rotY, int angle)
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

void Explosion::PushData(PolygonManager& polygons, MyGraphicsDevice& gDevice)
	{
		//drawing line segments from the center
		//so the number of sides need to be multiplied
		int numSides = (360 / ANGLE) * 2;
		
		std::vector<GLfloat> vertices;
		std::vector<GLfloat> colors;

		float oldX = 0.0f;
		float oldY = (float)RADIUS;


		//find the next n sides by rotating by objAngle
		//about the origin 0,0
		for(int i = 0; i < numSides; i++ )
		{
			//push center
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
			vertices.push_back(1.0f);
			
			//red
			colors.push_back(1.0f);
			colors.push_back(0.0f);
			colors.push_back(0.0f);
			colors.push_back(1.0f);
			centerIsRed = true;

			//push other end of line segment
			RotatePoints(oldX, oldY, oldX, oldY, ANGLE);
			vertices.push_back(oldX);
			vertices.push_back(oldY);
			vertices.push_back(0.0f);
			vertices.push_back(1.0f);

			//yellow
			colors.push_back(1.0f);
			colors.push_back(1.0f);
			colors.push_back(0.0f);
			colors.push_back(1.0f);
		}

		polyIndex = polygons.AddPolygon(numSides, vertices, colors, gDevice);
		if(polyIndex != -1)
		{
			polygons.TranslatePolygon(originX, originY, polyIndex, 0);
		}
		vertices.clear();
		colors.clear();
	}

void Explosion::Update(PolygonManager& polygons, MyGraphicsDevice& gDevice)
{
	timer <= 0 ? isAlive = false : timer--;
	colorTimer--;
	if( colorTimer <= 0 )
	{
		//reset timer
		colorTimer = COLOR_TIME;
		int numVertices = (360 / ANGLE) * 2;
		if( centerIsRed )//make it yellow
		{
			//make others red
			for( int i = 0; i < numVertices * 4; i+=4 )
			{
				//every alternate set of vertices is the center
				if( (i / 4) % 2 == 0 )
				{
					polygons.polygons[polyIndex].vertexColors[i] = 1.0f;
					polygons.polygons[polyIndex].vertexColors[i+1] = 1.0f;
					polygons.polygons[polyIndex].vertexColors[i+2] = 0.0f;	
				}
				else
				{
					polygons.polygons[polyIndex].vertexColors[i] = 1.0f;
					polygons.polygons[polyIndex].vertexColors[i+1] = 0.0f;
					polygons.polygons[polyIndex].vertexColors[i+2] = 0.0f;
				}
			}
			centerIsRed = false;
		}
		else
		{
			//make others yellow
			for( int i = 0; i < numVertices * 4; i+=4 )
			{
				//every alternate set of vertices is the center
				if( (i / 4) % 2 == 0 )
				{
					polygons.polygons[polyIndex].vertexColors[i] = 1.0f;
					polygons.polygons[polyIndex].vertexColors[i+1] = 0.0f;
					polygons.polygons[polyIndex].vertexColors[i+2] = 0.0f;	
				}
				else
				{
					polygons.polygons[polyIndex].vertexColors[i] = 1.0f;
					polygons.polygons[polyIndex].vertexColors[i+1] = 1.0f;
					polygons.polygons[polyIndex].vertexColors[i+2] = 0.0f;
				}
			}
			centerIsRed = true;
		}
		polygons.polygons[polyIndex].updated = true;
	}
}