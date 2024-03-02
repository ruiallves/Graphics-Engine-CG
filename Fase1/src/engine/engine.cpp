#define _NO_CRT_STDIO_INLINE
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#undef _NO_CRT_STDIO_INLINE

#include <math.h>
#include "build/world_config.h"

// Vari�veis da c�mara
float camx = 5.0f;
float camy = 5.0f;
float camz = 5.0f;
float lookAtx = 0.0f;
float lookAty = 0.0f;
float lookAtz = 0.0f;
float upx = 0.0f;
float upy = 1.0f;
float upz = 0.0f;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / (h);

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void drawAxis() {
	// put axis drawing in here
	glBegin(GL_LINES);
	// X axis in red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(
		-100.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);
	// Y Axis in Green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f,
		-100.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);
	// Z Axis in Blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f,
		-100.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();
}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camx, camy, camz,
		lookAtx, lookAty, lookAtz,
		upx, upy, upz);

	drawAxis();



	// End of frame
	glutSwapBuffers();
}



//// write function to process keyboard events
//
//void keyboardFunc(unsigned char key, int x, int y) {
//	switch (key) {
//	case 'a':
//		posx -= 0.1;
//		break;
//	case 'd':
//		posx += 0.1;
//		break;
//	case 's':
//		posz += 0.1;
//		break;
//	case 'w':
//		posz -= 0.1;
//		break;
//	case 'q':
//		angle -= 15;
//		break;
//	case 'e':
//		angle += 15;
//		break;
//	case 'i':
//		scalez += 0.1;
//		break;
//	case 'k':
//		scalez -= 0.1;
//		break;
//	case 'j':
//		scalex -= 0.1;
//		break;
//	case 'l':
//		scalex += 0.1;
//		break;
//	case 'u':
//		scaley -= 0.1;
//		break;
//	case 'o':
//		scaley += 0.1;
//		break;
//	case '+':
//		scalex += 0.1;
//		scaley += 0.1;
//		scalez += 0.1;
//		break;
//	case '-':
//		scalex -= 0.1;
//		scaley -= 0.1;
//		scalez -= 0.1;
//		break;
//	}
//	glutPostRedisplay();
//}




int initGlut(int argc, char** argv) {

	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("CG@DI-UM");

	// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);


	// put here the registration of the keyboard callbacks
	//glutKeyboardFunc(keyboardFunc);


	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);

	// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}

void configCam(World world) {
	if (world) {
		camx = world->camera.position[0];
		camy = world->camera.position[1];
		camz = world->camera.position[2];

		lookAtx = world->camera.lookAt[0];
		lookAty = world->camera.lookAt[1];
		lookAtz = world->camera.lookAt[2];

		upx = world->camera.up[0];
		upy = world->camera.up[1];
		upz = world->camera.up[2];
	}
	else {
		std::cout << ("Cannot read camera configuration from invalid 'world' object.\n");
	}
}

int main(int argc, char** argv) {
	char* filePath = "../test_1_1.xml";

	World world = newConfig();
	world = parseXmlFile(&world,filePath);
	configCam(world);
	
	initGlut(argc, argv);
}