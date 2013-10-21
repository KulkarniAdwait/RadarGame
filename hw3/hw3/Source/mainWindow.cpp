#define _CRT_SECURE_NO_WARNINGS

#include "../Headers/Header.h"
#include "../Headers/GraphicsDevice.h"
#include "../Headers/Radar.h"
#include "../Headers/GameConstants.h"
#include "../Headers/FlyingObject.h"
#include <GL/glui.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <list>

using namespace std;
GLUI *glui_b;
GLUI_Panel *scorePanel;
GLUI_StaticText *txtUpdateIndex;
MyGraphicsDevice gDevice;
PolygonManager polygonManager;
Radar *radar;
std::list<FlyingObject*> flyingObjects;
int index = 0, vertexCount = 0;
extern int DIFFICULTY = 10;
extern int MAX_UFO_COUNT = 15;
int score = 0;
const int SCR_FRIENDLY_HIT = -20;
const int SCR_ENEMY_HIT = 5;

//left click handles clicks on the canvas
void HandleMouse(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN && button == GLUT_LEFT_BUTTON )
	{	
		std::list<FlyingObject*>::iterator it = flyingObjects.begin();
		while( it != flyingObjects.end() )
		{
			//can destroy only if recognized
			if( (*it)->recognized && (**it).CheckHit(x, y, polygonManager, gDevice) )
			{
				//update score
				//friendly hit. decrese score
				if((*it)->isFriendly)
				{
					score += SCR_FRIENDLY_HIT;
				}
				//enemy hit. increase score
				else
				{
					score += SCR_ENEMY_HIT;
				}
				//destroy object
				//polygonManager.deletePolygon((*it)->getPolygonIndex(), gDevice);
				delete *it;
				//remove from list
				it = flyingObjects.erase(it);
				break;
			}
			++it;
		}	
	}
}

void Update()
{
	//create UFO's if there are too few
	if( flyingObjects.size() < MAX_UFO_COUNT )
	{
		int startSide = std::rand() % 4;
		FlyingObject *fo = new FlyingObject(startSide, polygonManager, gDevice);
		flyingObjects.push_back(fo);
		fo = NULL;
	}

	radar->Update(polygonManager);
	std::list<FlyingObject*>::iterator it = flyingObjects.begin();

	while( it != flyingObjects.end() )
	{
		(**it).Update(polygonManager, radar);
		//if object has reached end of its path then delete it
		if( (*it)->endReached )
		{
			//enemy escaped. decrese score
			if( (*it)->isFriendly == false )
			{
				score -= SCR_ENEMY_HIT;
			}
			
			//destroy object
			polygonManager.deletePolygon( (*it)->getPolygonIndex(), gDevice);
			delete *it;
			//remove from list
			it = flyingObjects.erase(it);
		}
		else
		{
			++it;
		}
	}
	txtUpdateIndex->set_text(std::to_string((long double)(score)).c_str());
	polygonManager.Update(gDevice);
}

void GameLoop( int t )
{
	Update();
	Display();
	glutTimerFunc( 1000 / FRAME_RATE, GameLoop, t ); 
}

void Display()
{
	gDevice.Display();
}

int main(int argc, char *argv[]) {

	InitGraphics(argc, argv);
	//allocate the buffers
	gDevice.Init();

	radar = new Radar();
	radar->PushData(polygonManager, gDevice);

	//start and direction must be selected randomly
	std::srand(std::time(NULL));

	//run game loop
	glutTimerFunc( 1000 / FRAME_RATE, GameLoop, 0 ); 
    // enter the event loop
	glutMainLoop();

    return 0;
}

void DifficultyManager()
{
	//increment the spped and the number off UFO's based on user performance here

}


void InitGraphics(int argc, char *argv[])
{
	// initialize glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(100, 100);
	int winHandler = glutCreateWindow("glMultiDrawElements Example");

	glui_b = GLUI_Master.create_glui_subwindow(winHandler, GLUI_SUBWINDOW_BOTTOM );
	scorePanel = new GLUI_Panel(glui_b, "Score");
	//txtUpdateIndex = new GLUI_StaticText(draw_panel, "");
	txtUpdateIndex = glui_b->add_statictext_to_panel(scorePanel, (std::to_string((long double)(score))).c_str());

	// specify glut call backs
	glutDisplayFunc(Display);
	glutMouseFunc(HandleMouse);

	// check to see opengl 2.0 is available using glew
	glewInit();

	const char* version = (const char*)glGetString(GL_VERSION); 
    cout << "OpenGL Version: " << string(version) << endl;

    version = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION); 
    cout << "GLSL Version: " << string(version) << endl;	

	// initialize shaders
	GLuint shader_program = InitShader("draw.vert", "draw.frag");
	
	// activate shader program
	glUseProgram(shader_program);
}

GLuint InitShader(const char* vs_file, const char* fs_file) {

	char *vs_str = NULL, *fs_str = NULL;

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

	vs_str = TextFileRead(vs_file);
	fs_str = TextFileRead(fs_file);

	const char *vv = vs_str;
	const char *ff = fs_str;

	glShaderSource(vs, 1, &vv, NULL);
	glShaderSource(fs, 1, &ff, NULL);

	free(vs_str); free(fs_str); 

	glCompileShader(vs);

	int IsCompiled;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &IsCompiled);
	cout << "Compiled VS:" << IsCompiled << endl << flush;
	if(!IsCompiled)
    {
		int maxLength;
       glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &maxLength);
 
       /* The maxLength includes the NULL character */
       char *vertexInfoLog = (char *)malloc(maxLength);
 
       glGetShaderInfoLog(fs, maxLength, &maxLength, vertexInfoLog);
 
    	cout << vertexInfoLog << endl;
       /* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
       /* In this simple program, we'll just leave */
       	free(vertexInfoLog);

		cout << "Vertex Shader Failure!" << endl;
//		return -1;
    }
	glCompileShader(fs);
	IsCompiled = 0;
	glGetShaderiv(fs, GL_COMPILE_STATUS, &IsCompiled);

	cout << "Compiled FS:" << IsCompiled << endl << flush;
	if(!IsCompiled)
    {
		int maxLength;
       glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &maxLength);
 
       /* The maxLength includes the NULL character */
       char *fragmentInfoLog = (char *)malloc(maxLength);
 
       glGetShaderInfoLog(fs, maxLength, &maxLength, fragmentInfoLog);
 
    cout << fragmentInfoLog << endl;
						// Handle the error in an appropriate way such as 
						// displaying a message or writing to a log file. */
       /* In this simple program, we'll just leave */
       free(fragmentInfoLog);
		cout << "Fragment Shader Failure!" << endl;
       return -1;
    }

	glEnable( GL_DEPTH_TEST );

//	printShaderInfoLog(vs);
//	printShaderInfoLog(fs);

	GLuint shader_program = glCreateProgram();
	glAttachShader(shader_program, vs);
	glAttachShader(shader_program, fs);

	glBindAttribLocation(shader_program, 0, "vPosition");
	glBindAttribLocation(shader_program, 1, "vColor");

	glLinkProgram(shader_program);

	glUseProgram(shader_program);
	
						// set up attribute locations, color and position
	GLint vPosition = glGetAttribLocation( shader_program, "vPosition" );
	GLint vColor    = glGetAttribLocation( shader_program, "vColor" );
	GLint uWorldToDevice = glGetUniformLocation( shader_program, "uWorldToDevice" );

	//gDevice = MyGraphicsDevice(vPosition, vColor);
	gDevice.vPosition = vPosition;
	gDevice.vColor = vColor;
	gDevice.uWorldToDevice = uWorldToDevice;

	return shader_program;
}

char* TextFileRead(const char *fn) {


	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL) {
		fp = fopen(fn,"rt");

		if (fp != NULL) {
      
      fseek(fp, 0, SEEK_END);
      count = ftell(fp);
      rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	for (int k = 0; k < count; k++)
		cout << content[k];
	return content;
}