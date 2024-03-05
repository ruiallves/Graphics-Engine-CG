#define _NO_CRT_STDIO_INLINE
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
//#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#undef _NO_CRT_STDIO_INLINE

#include "math.h"
#include "../engine/config/world_config.h"
#include "../tools/figura.h"

using namespace std;

float red = 1;
float green = 1;
float blue = 1;

float xx = 0.0f, yy = 0.0f, zz = 0.0f;
float angleY = 0.0f, angleX = 0.0f;
float camx = 5.0f;
float camy = 5.0f;
float camz = 5.0f;
float lookAtx = 0.0f;
float lookAty = 0.0f;
float lookAtz = 0.0f;
float upx = 0.0f;
float upy = 1.0f;
float upz = 0.0f;

vector<Figura> figuras;

GLenum mode = GL_LINE;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

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

void prepareData(World world) {

	for (int i = 0; i < world->numFiles; ++i) {
		string filepath = "../../../outputs/" + world->files[i];
		ifstream file(filepath);
		if (!file) {
			cerr << "Erro ao abrir o arquivo: " << world->files[i] << endl;
			return;
		}

		float x, y, z;
		vector<Vertice> vertices;

		string line;
		while (getline(file, line)) {
			istringstream iss(line);
			string token;
			float coord;
			vector<float> coords;

			while (getline(iss, token, ',')) {
				istringstream(token) >> coord;
				coords.push_back(coord);
			}

			if (coords.size() != 3) {
				cerr << "Erro ao ler linha do arquivo: " << world->files[i] << endl;
				continue;
			}

			vertices.push_back(newVertice(coords[0], coords[1], coords[2]));
		}

		Figura fig = newFigura();
		fig->vertices = vertices;
		figuras.push_back(fig);

		file.close();
	}
}

void drawVertices() {
	glBegin(GL_TRIANGLES);
	glColor3f(red, green, blue);

	for (size_t i = 0; i < figuras.size();i++) {
		Figura fig = figuras[i];
		for (size_t j = 0; j < fig->vertices.size();j++) {
			Vertice atualVertice = fig->vertices[j];
			glVertex3f(atualVertice->x, atualVertice->y, atualVertice->z);
		}
	}

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
	glPolygonMode(GL_FRONT_AND_BACK, mode);
	glTranslatef(xx, yy, zz);
	glRotatef(angleY, 0.0, 1.0, 0.0);
	glRotatef(angleX, 1.0, 0.0, 0.0);
	drawVertices();


	// End of frame
	glutSwapBuffers();
}

//// write function to process keyboard events
void keyboardFunc(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
		case 'A':
			angleY -= 10;
			break;

	case 'd':
		case 'D':
			angleY += 10;
			break;

	case 'w':
		case 'W':
			angleX -= 10;
			break;

	case 's':
		case 'S':
			angleX += 10;
			break;
	
	case 'p':
		case 'P':
			mode = GL_POINT;
			break;

	case 'l':
		case 'L':
			mode = GL_LINE;
			break;

	case 'f':
		case 'F':
			mode = GL_FILL;
			break;

	case '-':
		camx += 2;
		camy += 2;
		camz += 2;
		break;

	case '+':
		camx -= 2;
		camy -= 2;
		camz -= 2;
		break;
	}

	glutPostRedisplay();
}

void keyboardspecial(int key, int x, int y) {
	switch (key) {
		case GLUT_KEY_UP:
			yy += 1;
			break;
		case GLUT_KEY_DOWN:
			yy -= 1;
			break;
		case GLUT_KEY_LEFT:
			xx -= 1;
			break;
		case GLUT_KEY_RIGHT:
			xx += 1;
			break;
	}
	glutPostRedisplay();
}


int initGlut(int argc, char** argv, World world) {

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
	glutKeyboardFunc(keyboardFunc);
	glutSpecialFunc(keyboardspecial);

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
	if (argc < 2) {
		std::cerr << "Usage: " << "./engine.exe" << " <filename>" << std::endl;
		return 1;
	}
	std::string filepath = argv[1];
	World world = newConfig();
	world = parseXmlFile(&world, ("../../../tests/" + filepath).c_str());
	configCam(world);
	prepareData(world);
	initGlut(argc, argv, world);

	return 0;
}
