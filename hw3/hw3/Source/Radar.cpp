#include "../Headers/Radar.h"

Radar::Radar(PolygonManager& polygons, MyGraphicsDevice& gDevice)
{
	//poly = NULL;
	polyIndex = -1;
	angle = -3.0f;
	radius = SCREEN_WIDTH / 2;
		
	//origin
	radarVertices.push_back(SCREEN_WIDTH / 2.0);
	radarVertices.push_back(SCREEN_HEIGHT / 2.0 - SHIFT_DUE_TO_UI);
	radarVertices.push_back(0.0f);
	radarVertices.push_back(1.0f);

	radarVertices.push_back(SCREEN_WIDTH / 2.0);
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

	for( int i = 0; i < TRACE_VERT_SIZE - 1; i++ )
	{
		RotatePoints(oldX, oldY, oldX, oldY, 360.0f / TRACE_VERT_SIZE);

		traceVertices.push_back(oldX);
		traceVertices.push_back(oldY);
		traceVertices.push_back(0.0f);
		traceVertices.push_back(1.0f);
	
		traceColors.push_back(0.0f);
		traceColors.push_back(1.0f);
		traceColors.push_back(0.5f);
		traceColors.push_back(0.1f);
	}

	PushData(polygons, gDevice);
}

void Radar::PushData(PolygonManager& polygons, MyGraphicsDevice& gDevice)
{
	polyIndex = polygons.AddPolygon(2, radarVertices, radarColors, gDevice);
	radarVertices.clear();
	radarColors.clear();

	polygons.AddPolygon(TRACE_VERT_SIZE, traceVertices, traceColors, gDevice);
	traceVertices.clear();
	traceColors.clear();
}

void Radar::Update(PolygonManager& polygons)
{
	if(polyIndex != -1)
	{
		polygons.RotatePolygon(0, polyIndex, angle);

		//floatingpoint rotation is throwing it off center
		polygons.polygons[polyIndex].vertexData[0] = origX;
		polygons.polygons[polyIndex].vertexData[1] = origY;
		//x is at 4
		radarVec.x = polygons.polygons[polyIndex].vertexData[4] - origX;
		//y is at 5
		radarVec.y = polygons.polygons[polyIndex].vertexData[5] - origY;
	}
}

