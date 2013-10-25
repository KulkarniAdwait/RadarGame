#include "../Headers/GraphicsDevice.h"

//draws the polygons stored in the GPU
void MyGraphicsDevice::Display()
{    
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(2.0f);

	if(polygonCount)
	{
		glBindVertexArray(vao);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDICES]);
		glMultiDrawElements(GL_LINE_LOOP, counts._Myfirst, GL_UNSIGNED_INT, (const GLvoid **)IBO_pointer_indices._Myfirst, polygonCount);
		//glMultiDrawElements(GL_TRIANGLE_FAN, counts._Myfirst, GL_UNSIGNED_INT, (const GLvoid **)IBO_pointer_indices._Myfirst, polygonCount);
	}	
	glFlush();
	//glutPostRedisplay(); 
	glutSwapBuffers();
}

//send data to GPU
void MyGraphicsDevice::PushData(int vertexCount, std::vector<GLfloat> vertices, std::vector<GLfloat> colors, GLintptr* retVertexLocation, GLintptr* retColorLocation, int* countLocation)
{
	*countLocation = counts.size();
	counts.push_back(vertexCount);
	
	//stores the number of bytes we had added till the (n-1)th polygon
	//these are the pointers into the index array
	IBO_pointer_indices.push_back(indexOffset);

	//push data to GPU
	glBindVertexArray(vao);
	//vertex
	GLsizei vertexChangeSize = vertexCount * 4 * SIZE_OF_GL_FLOAT;
	glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTICES]);
	glBufferSubData(GL_ARRAY_BUFFER, vertexOffset, vertexChangeSize, vertices._Myfirst);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);
	*retVertexLocation = vertexOffset;
	vertexOffset += vertexChangeSize;
	//vertexChangeSize = 0;

	//color
	GLsizei colorChangeSize = vertexCount * 4 * SIZE_OF_GL_FLOAT;
	glBindBuffer(GL_ARRAY_BUFFER, vbo[COLORS]);
	glBufferSubData(GL_ARRAY_BUFFER, colorOffset, colorChangeSize, colors._Myfirst);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);
	*retColorLocation = colorOffset;
	colorOffset += colorChangeSize;
	//colorChangeSize = 0;

	//index
	GLsizei indexChangeSize = vertexCount * sizeof(GLuint);
	std::vector<GLuint> indices;
	for(int i = 0; i < vertexCount; i++ )
	{
		indices.push_back(iIndices++);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDICES]);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, indexOffset, indexChangeSize, indices._Myfirst);
	indexOffset += indexChangeSize;
	//indexChangeSize = 0;

	polygonCount++;
}

//update data in the GPU
void MyGraphicsDevice::UpdateData(PolygonData poly)
{
	//push data to GPU
	glBindVertexArray(vao);
	//vertex
	GLsizei vertexChangeSize = poly.vertexCount * 4 * SIZE_OF_GL_FLOAT;
	glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTICES]);
	glBufferSubData(GL_ARRAY_BUFFER, poly.location, vertexChangeSize, poly.vertexData._Myfirst);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);

	//color
	GLsizei colorChangeSize = poly.vertexCount * 4 * SIZE_OF_GL_FLOAT;
	glBindBuffer(GL_ARRAY_BUFFER, vbo[COLORS]);
	glBufferSubData(GL_ARRAY_BUFFER, poly.colorLocation, colorChangeSize, poly.vertexColors._Myfirst);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vColor);

}