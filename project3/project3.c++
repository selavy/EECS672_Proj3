// project2.c++: Starter for EECS 672 Project 2

#include <GL/gl.h>
#include <GL/freeglut.h>

#include "Controller.h"
#include "Square.h"

#define WHITE 1
#define BLACK 0
#define BORDER 2
#define TEST 3
#define FLIP( x ) do{ (x) ^= 1; } while(0)

#define __DEBUG__

int main(int argc, char* argv[])
{
	// One-time initialization of the glut
	glutInit(&argc, argv);

	Controller c("Your move.", GLUT_DEPTH | GLUT_DOUBLE);

	// create your scene, adding things to the Controller....
	vec3 location = { 0.0f, 0.0f, 0.0f };

	c.addModel( new Square( TEST, location, 1.0f, 1.0f ) );

	glClearColor(1.0, 1.0, 1.0, 1.0);

	// Off to the glut event handling loop:
	glutMainLoop();

	return 0;
}

