#ifndef GL_DEFS_H

#define GL_DEFS_h

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

GLuint InitShader(const char* vs_file, const char* fs_file);

char* TextFileRead(const char *fn);

//Adwait Kulkarni
void HandleMouse(int button, int state, int x, int y);
void Display();
void Update();
void GameLoop( int t );
void InitGraphics(int argc, char *argv[]);
void DifficultyManager();

#endif
