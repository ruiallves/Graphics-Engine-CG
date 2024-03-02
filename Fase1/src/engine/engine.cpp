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

#include <math.h>
#include "build/world_config.h"
#include "build/figura.h"

using namespace std;

vector<Vertice> vertices;

// Variáveis da câmara
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

//std::vector<Vertice> gerarCone(float raio, float altura, int slices, int stacks) {
//
//	// Calcular o ângulo entre cada fatia
//	float sliceAngle = 2.0f * 3.14159 / slices;
//
//	// Calcular a altura de cada stack
//	float stackHeight = altura / stacks;
//
//	// Gerar os vértices para cada stack
//	for (int i = 0; i <= stacks; ++i) {
//		float y = -altura / 2.0f + i * stackHeight; 
//		float radius = raio * (1.0f - static_cast<float>(i) / stacks);
//
//		// Gerar os vértices para cada fatia
//		for (int j = 0; j < slices; ++j) {
//			float angle = j * sliceAngle;
//			float x = radius * cos(angle); 
//			float z = radius * sin(angle); 
//
//			vertices.push_back(newVertice(x, y, z));
//		}
//	}
//
//	// Adicionar o vértice do topo do cone
//	vertices.push_back(newVertice(0.0f, altura / 2.0f, 0.0f));
//
//	return vertices;
//}
//
//void gerarPlano(float tamanho, int divisoes) {
//	// Limpar os vértices existentes
//	vertices.clear();
//
//	// Calcular o número total de vértices
//	int numVertices = (divisoes + 1) * (divisoes + 1);
//	// Redimensionar o vetor de vértices
//	vertices.resize(numVertices);
//
//	// Calcular o espaçamento entre os vértices
//	float espacamento = tamanho / divisoes;
//
//	// Preencher os vértices do plano
//	int index = 0;
//	for (int i = 0; i <= divisoes; ++i) {
//		for (int j = 0; j <= divisoes; ++j) {
//			// Calcular as coordenadas x e z do vértice
//			float x = j * espacamento - tamanho / 2.0f;
//			float z = i * espacamento - tamanho / 2.0f;
//			// Definir a coordenada y como 0 (plano no plano xy)
//			float y = 0.0f;
//
//			// Atribuir as coordenadas ao vértice atual
//			vertices[index++] = newVertice(x, y, z);
//		}
//	}
//}

void prepareData(World world) {

	for (int i = 0; i < world->numFiles; ++i) {
		// Criar uma cópia da string atual para evitar modificar a original
		std::string filename = world->files[i];

		// Substituir os underscores por espaços em branco
		for (char& c : filename) {
			if (c == '_') {
				c = ' ';
			}
		}

		std::istringstream iss(filename);

		std::string prefix;
		std::string file_extension;
		iss >> prefix;

		switch (prefix[0]) {
			
			case 'c': //cone
				/*int radius, height, slices, stacks;
				iss >> radius >> height >> slices >> stacks >> file_extension;
				vertices = gerarCone(radius, height, slices, stacks);*/
				break;
			
			case 's': //esfera
				break;

			case 'b': //box
				break;

			case 'p': //plane
				/*int tamanho, divisoes;
				iss >> tamanho >> divisoes;
				gerarPlano(tamanho, divisoes);*/
				break;

			default:
				std::cerr << "Formato de arquivo inválido: " << world->files[i] << std::endl;
				break;
		}


	}

	// Fazer algo com os vértices extraídos, como armazená-los no mundo
	// por exemplo, world.vertices = vertices;
}

//void desenharFigura() {
//	
//	for (size_t i = 0; i < vertices.size(); i+=3) {
//		glBegin(GL_TRIANGLES);
//		Vertice v1 = vertices[i];
//		glVertex3f(v1->x, v1->y, v1->z);
//		Vertice v2 = vertices[i+1];
//		glVertex3f(v2->x, v2->y, v2->z);
//		Vertice v3 = vertices[i + 2];
//		glVertex3f(v3->x, v3->y, v3->z);
//		glEnd();
//	}
//	
//}



void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camx, camy, camz,
		lookAtx, lookAty, lookAtz,
		upx, upy, upz);

	drawAxis();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//desenharFigura();


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




int initGlut(int argc, char** argv, World world) {

	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("CG@DI-UM");

	prepareData(world);

	// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);


	// put here the registration of the keyboard callbacks
	//glutKeyboardFunc(keyboardFunc);


	//  OpenGL settings
	//glEnable(GL_DEPTH_TEST);

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
	prepareData(world);
	initGlut(argc, argv, world);

	return 0;
}