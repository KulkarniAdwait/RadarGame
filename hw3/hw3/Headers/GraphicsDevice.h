#ifndef MY_GRAPHICS_DEVICE_H
#define MY_GRAPHICS_DEVICE_H

#define VERTICES 0
#define COLORS 1
#define INDICES 2

#include "Header.h"
#include "GameConstants.h"
#include "polygonData.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>

class MyGraphicsDevice
{
private:
	GLuint vao;
	GLuint vbo[3];
	
	std::vector<GLsizei> counts;
	std::vector<GLuint> IBO_pointer_indices;
	//current positions in the respective buffers
	GLintptr vertexOffset, colorOffset, indexOffset;
	int iIndices;

	static const int SIZE_OF_GL_FLOAT = sizeof(GLfloat);
	int polygonCount;
	glm::mat4x4 worldToDevice;

	void InitBuffers()
	{
		//clear color set to white
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		glGenVertexArrays(1, &vao);
		glGenBuffers(3, vbo);

		//allocate empty buffers of apt. size
		glBindVertexArray(vao);
		//vertices
		glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTICES]);	
		glBufferData(GL_ARRAY_BUFFER, MAX_VERTS * SIZE_OF_GL_FLOAT, NULL, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(vPosition);
		// colors
		glBindBuffer(GL_ARRAY_BUFFER, vbo[COLORS]);	
		glBufferData(GL_ARRAY_BUFFER, MAX_VERTS * SIZE_OF_GL_FLOAT, NULL, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(vColor);
		//indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDICES]);	
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_VERTS * sizeof(GLuint), NULL, GL_DYNAMIC_DRAW);
	}

	//Adwait Kulkarni
	//Generates the World to Device co ordinate transform Matrix and pushes to uniform in GPU
	void GenerateWorldToDevice()
	{
		using namespace glm;
		mat4x4 TworldToOrigin, SworldToUnit, StoDevice, TdeviceOrigin;
		//all matrices in column major
		//move to origin
		TworldToOrigin = mat4x4(1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							-0.0f, -0.0f, 0.0f, 1.0f);

		//to convert to unit scale by 1/worldMax
		SworldToUnit = mat4x4(1.0f/(float)(SCREEN_WIDTH), 0.0f, 0.0f, 0.0f,
							0.0f, -1.0f/(float)(SCREEN_HEIGHT), 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f);

		//scale up to the device max
		StoDevice = mat4x4(2.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 2.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 0.0f, 1.0f);

		//translate to device x, y
		TdeviceOrigin = mat4x4(1.0f, 0.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f, 0.0f,
							0.0f, 0.0f, 1.0f, 0.0f,
							-1.0f, 1.0f, 0.0f, 1.0f);

		worldToDevice = TdeviceOrigin * StoDevice * SworldToUnit * TworldToOrigin;

		//push to GPU
		if(uWorldToDevice != -1)
		{
			glUniformMatrix4fv(uWorldToDevice, 1, GL_FALSE, value_ptr(worldToDevice));
		}
	}

	//Adwait Kulkarni
	//Converts the input co ordinates from Screen to world co ordinates using transform Matrix created in GenerateScreenToWorld()
	//void world_to_device (int x, int y, float *world_x, float *world_y)
	//{
	//	glm::vec4 deviceCoords, worldCoords;
	//	worldCoords = glm::vec4(x, y, 0, 1);
	//	deviceCoords = worldToDevice * worldCoords;
	//	*world_x = deviceCoords.x;
	//	*world_y = deviceCoords.y;
	//}
public:
	GLint vPosition, vColor, uWorldToDevice;

	MyGraphicsDevice()
	{
		this->vPosition = -1;
		this->vColor = -1;
		this->uWorldToDevice = -1;
		GLuint vao = -1;
		GLuint vbo[3] = {-1, -1, -1};
		polygonCount = 0;
		vertexOffset = 0, colorOffset = 0, indexOffset = 0;
		iIndices = 0;
	}

	~MyGraphicsDevice()
	{

	}

	//Generates and allocates empty buffers on GPU
	void Init()
	{
		InitBuffers();
		GenerateWorldToDevice();
	}

	//draws the polygons stored in the GPU
	void Display();

	//send data to GPU
	void PushData(int vertexCount, std::vector<GLfloat> vertices, std::vector<GLfloat> colors, GLintptr* retVertexLocation, GLintptr* retColorLocation, int* countLocation);
	
	//send data to GPU
	void UpdateData(PolygonData poly);

	void DeletePolygon(int countIndex)
	{
		counts[countIndex] = 0;
	}
};

#endif