#define _NO_CRT_STDIO_INLINE

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <../glew/GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "math.h"
#include "../engine/config/world_config.h"
#include "../tools/figura.h"
#include <iomanip>
#define NOW ((1.0f*glutGet(GLUT_ELAPSED_TIME))/1000.0f)
#define PI 3.14159265358979323846
using namespace std;

World world = newConfig();

float red = 1;
float green = 1;
float blue = 1;

float alpha = PI / 4;
float beta_ = PI / 4;
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

GLenum mode = GL_LINE;

int nrFiguras = 0;
vector<int> sizBuffers;
GLuint* buffers = NULL;

int frames = 0;
int timebase = 0;
float tempo_inicial = 0.0f;

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

void loadBuffers(Arvore groups, int* index) {
	if (groups) {
		Group group = (Group)getDataArvore(groups);
		LinkedList models = getGroupFigures(group);

		Figura fig = (Figura)getData(models);
		cout << getTotalVertices(fig) << endl;

		/*while (models != nullptr) {
			vector<float> toBuffer = figuraToVector(fig);
			glBindBuffer(GL_ARRAY_BUFFER, buffers[(*index)++]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * toBuffer.size(), toBuffer.data(), GL_STATIC_DRAW);
			sizBuffers.push_back(toBuffer.size() / 3);
			fig = (Figura)getData((LinkedList)getNext(models));
		}*/

		/*LinkedList filhos = getFilhosArvore(groups);
		for (unsigned long i = 0; i < getSizeOfFiguras(filhos); i++) {
			Arvore next = (Arvore)getListElemAt(filhos, i);
			loadBuffers(next, index);
		}*/
	}
}

void drawVertices(LinkedList figuras) {
	// Verifica se a lista de figuras está inicializada
	if (figuras == nullptr) {
		std::cerr << "Lista de figuras não inicializada." << std::endl;
		return;
	}

	// Itera sobre cada figura na lista
	LinkedList currentFigura = figuras;
	while (currentFigura != nullptr) {
		Figura figura = (Figura)getData(currentFigura);
		if (figura != nullptr) {
			// Inicia o desenho dos triângulos da figura atual
			glBegin(GL_TRIANGLES);

			// Obtém a lista de vértices da figura
			LinkedList vertices = getFiguraVertices(figura);
			LinkedList currentVertice = vertices;

			// Desenha cada vértice da figura atual
			while (currentVertice != nullptr) {
				Vertice vertice = (Vertice)getData(currentVertice);
				if (vertice != nullptr) {
					float x = getVerticeX(vertice);
					float y = getVerticeY(vertice);
					float z = getVerticeZ(vertice);
					glVertex3f(x, y, z);
				}
				currentVertice = (LinkedList)getNext(currentVertice);
			}

			// Finaliza o desenho dos triângulos da figura atual
			glEnd();
		}
		// Avança para a próxima figura na lista
		currentFigura = (LinkedList)getNext(currentFigura);
	}
}

// VERSÃO ALTERNATIVA (TAMBEM NAO FUNCIONA)
void drawGroups(Arvore groups) {
	if (groups) {
		glPushMatrix(); // guarda o estado dos eixos

		Group group = (Group)getDataArvore(groups);
		LinkedList transforms = getGroupTransforms(group), models = getGroupFigures(group);

		LinkedList currentTransform = transforms;
		while (currentTransform != nullptr) {
			Transform t = (Transform)getData(currentTransform);
			if (t != nullptr) {
				float x = getTransformX(t);
				float y = getTransformY(t);
				float z = getTransformZ(t);
				char tr_type = getTransformType(t);
				if (tr_type == 'r') {
					float angle = getTransformAngle(t);
					glRotatef(angle, x, y, z);
				}
				else if (tr_type == 't') {
					glTranslatef(x, y, z);
				}
				else if (tr_type == 's') {
					glScalef(x, y, z);
				}
			}
			currentTransform = (LinkedList)getNext(currentTransform);
		}

		// Desenho das figuras
		glBegin(GL_TRIANGLES);
		drawVertices(models);
		glEnd();

		// Procede para fazer o mesmo aos nodos filho. 
		LinkedList child = (LinkedList)getFilhosArvore(groups);
		for (unsigned long i = 0; i < getSizeOfFiguras(child);i++) {
			Arvore next = (Arvore)getListElemAt(child, i);
			drawGroups(next);
		}
		glPopMatrix(); // retorna ao respetivo estado anterior dos eixos.

	}
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camx, camy, camz,
		lookAtx, lookAty, lookAtz,
		upx, upy, upz);

	//drawAxis();
	glColor3f(1.0f, 1.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, mode);	
	//printWorld(world);
	drawGroups(getWorldGroups(world));

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
		camy += 2; 
		break;
	case GLUT_KEY_DOWN:
		camy -= 2; 
		break;
	case GLUT_KEY_LEFT:
		camx += 2; 
		break;
	case GLUT_KEY_RIGHT:
		camx -= 2;
		break;
	}
	glutPostRedisplay();
}


int initGlut(int argc, char** argv, World world) {

	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(getWindowWidth(getWorldWindow(world)), getWindowHeight(getWorldWindow(world)));
	glutCreateWindow("Fase 3");

	// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	glewInit();
	// put here the registration of the keyboard callbacks
	glutKeyboardFunc(keyboardFunc);
	glutSpecialFunc(keyboardspecial);

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_VERTEX_ARRAY);
	glPolygonMode(GL_FRONT, GL_LINE);
	glGenBuffers(nrFiguras, buffers);

	int indice = 0;
	loadBuffers(getWorldGroups(world), &indice);

	frames++;
	int time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		float fps = frames * 1000.0f / (time - timebase);
		std::ostringstream titleStream;
		titleStream << "FPS: " << std::fixed << std::setprecision(2) << fps;
		titleStream << ", PCAM: (" << std::fixed << std::setprecision(2) << camx << ", " << camy << ", " << camz << ")";
		titleStream << ", LA: (" << std::fixed << std::setprecision(2) << lookAtx << ", " << lookAty << ", " << lookAtz << ")";
		titleStream << ", alpha = " << std::fixed << std::setprecision(2) << alpha;
		titleStream << ", beta = " << std::fixed << std::setprecision(2) << beta_;
		//titleStream << ", CamMode: " << (mode ? "POINT" : "LINE");
		std::string title = titleStream.str();
		glutSetWindowTitle(title.c_str());
		timebase = time;
		frames = 0;
	}


	// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}

void configCam(World world) {
	if (world) {
		
		camx = getCameraPosition(getWorldCamera(world))[0];
		camy = getCameraPosition(getWorldCamera(world))[1];
		camz = getCameraPosition(getWorldCamera(world))[2];

		lookAtx = getCameraLookAt(getWorldCamera(world))[0];
		lookAty = getCameraLookAt(getWorldCamera(world))[1];
		lookAtz = getCameraLookAt(getWorldCamera(world))[2];

		upx = getCameraUp(getWorldCamera(world))[0];
		upy = getCameraUp(getWorldCamera(world))[1];
		upz = getCameraUp(getWorldCamera(world))[2];
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
	world = parseXmlFile(&world, ("../../../tests/" + filepath).c_str());

	configCam(world);
	//printWorld(world);

	nrFiguras = getSizeOfFiguras(getGroupFigures((Group)getDataArvore(getWorldGroups(world))));
	//printf("Nr de figuras: %d\n", nrFiguras);

	buffers = (GLuint*)malloc(nrFiguras * sizeof(GLuint));
	timebase = glutGet(GLUT_ELAPSED_TIME);
	tempo_inicial = NOW;
	initGlut(argc, argv, world);

	return 0;
}
