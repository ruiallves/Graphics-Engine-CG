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
#include "../tools/matrix.h"

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
vector<int> sizBuffers; // Guarda o tamanho de cada buffer em termos de número de vértices
GLuint* buffers = NULL; // Guarda os identificadores dos buffers
char title[128];
int frames = 0;
int timebase = 0;
float tempo_inicial = 0.0f;

bool showC = false;

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
	gluPerspective(getProjectionFOV(getCameraProjection(getWorldCamera(world))), ratio, getProjectionNear(getCameraProjection(getWorldCamera(world))), getProjectionFar(getCameraProjection(getWorldCamera(world))));

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

		while (models != nullptr) {
			Figura fig = (Figura)getData(models);
			// Converte a figura para um vetor de floats para serem usados nos VBOs
			vector<float> toBuffer = figuraToVector(fig);
			// Associa um buffer a um identificador e incrementa o índice
			glBindBuffer(GL_ARRAY_BUFFER, buffers[(*index)++]);
			// Copia os dados da figura para o buffer OpenGL
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * toBuffer.size(), toBuffer.data(), GL_STATIC_DRAW);
			// Guarda o tamanho do buffer em termos de número de vértices (cada vértice tem 3 floats: x, y, z)
			sizBuffers.push_back(toBuffer.size() / 3);
			models = (LinkedList)getNext(models);
		}

		LinkedList filhos = getFilhosArvore(groups);
		for (unsigned long i = 0; i < getSizeOfFiguras(filhos); i++) {
			Arvore next = (Arvore)getListElemAt(filhos, i);
			loadBuffers(next, index);
		}
	}
}

void drawCatmullRomCurve(const std::vector<std::vector<float>>& controlPoints) {
	// Define o número de pontos de amostra ao longo da curva
	const int numSamples = 100;

	// Calcula o intervalo de amostragem
	const float sampleInterval = 1.0f / numSamples;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i <= numSamples; ++i) {
		// Calcula o parâmetro t para este ponto de amostra
		float t = i * sampleInterval;

		// Calcula as coordenadas (x, y, z) do ponto de amostra usando a função getGlobalCatmullRomPoint
		float pos[3];
		float deriv[3];
		getGlobalCatmullRomPoint(t, controlPoints, pos, deriv);

		// Desenha o ponto de amostra
		glVertex3f(pos[0], pos[1], pos[2]);
	}

	// Finalizando o desenho da curva
	glEnd();
}


void execTransforms(LinkedList transforms, int* indice) {
	LinkedList currentTransform = transforms;

	while (currentTransform != nullptr) {
		Transform transf = (Transform)getData(currentTransform);
		if (transf != nullptr) {
			switch (getTransformType(transf)) {
			case 'r': {
				float angle = getTransformAngle(transf);
				float time = getTransformTime(transf);
				if (time != 0) {
					angle = fmod(angle + (NOW - tempo_inicial) * 360 / time, 360);
				}
				glRotatef(angle, getTransformX(transf), getTransformY(transf), getTransformZ(transf));
				break;
			}
			case 's': {
				glScalef(getTransformX(transf), getTransformY(transf), getTransformZ(transf));
				break;
			}
			case 't': {
				// Obtém o tempo de transformação
				float time = getTransformTime(transf);

				// Verifica se a transformação envolve uma interpolação ao longo do tempo
				if (time > 0) {
					// Arrays para armazenar a posição e a derivada da curva
					float pos[3], deriv[3], y[3], z[3], rot[16];

					// Obtém os pontos de controle para a curva Catmull-Rom
					vector<vector<float>> points = transPoints(transf);

					// Calcula a posição e a derivada na curva Catmull-Rom para o tempo atual
					getGlobalCatmullRomPoint(NOW / time, points, pos, deriv);

					// Desenha a curva se a flag showC estiver ativada
					if (showC) drawCatmullRomCurve(points);

					// Translada o objeto para a posição calculada
					glTranslatef(pos[0], pos[1], pos[2]);

					// Verifica se a transformação deve alinhar com a trajetória
					if (getTransformAlign(transf)) {
						// Normaliza a derivada para obter a direção da trajetória
						normalize(deriv);

						// Calcula o vetor perpendicular à direção da trajetória e ao eixo Y atual
						cross(deriv, transformYAxis(transf).data(), z);
						normalize(z);  // Normaliza o vetor resultante

						// Calcula o vetor Y corrigido para manter a ortogonalidade
						cross(z, deriv, y);
						setTransformYAxis(transf, y[0], y[1], y[2]);
						normalize(y);  // Normaliza o vetor Y corrigido

						// Constrói a matriz de rotação com os vetores ortogonais
						buildRotMatrix(deriv, y, z, rot);

						// Aplica a matriz de rotação
						glMultMatrixf(rot);
					}
				}
				else {
					// Translação simples sem interpolação ao longo do tempo
					glTranslatef(getTransformX(transf), getTransformY(transf), getTransformZ(transf));
				}
				break;
			}

			default:
				break;
			}
		}
		currentTransform = (LinkedList)getNext(currentTransform);
	}
}

void drawGroups(Arvore groups, int* indice) {
	if (groups) {
		glPushMatrix();

		Group group = (Group)getDataArvore(groups);
		LinkedList transforms = getGroupTransforms(group), models = getGroupFigures(group);
		execTransforms(transforms, indice);

		for (unsigned long i = 0; i < getSizeOfFiguras(models); i++, (*indice)++) {
			// Associa o buffer atual ao buffer de vértices atualmente ativo
			glBindBuffer(GL_ARRAY_BUFFER, buffers[*indice]);
			// Define como os dados do buffer de vértices são interpretados
			glVertexPointer(3, GL_FLOAT, 0, 0);
			// Desenha a figura atual usando os vértices presentes no buffer atualmente ativo
			// GL_TRIANGLES indica que os vértices devem ser interpretados como triângulos
			// 0 indica que o primeiro vértice no buffer deve ser usado como início
			// sizBuffers[*indice] indica o número de vértices a serem desenhados, determinando o final do buffer
			glDrawArrays(GL_TRIANGLES, 0, sizBuffers[*indice]);
		}

		// Procede para fazer o mesmo aos nodos filho. 
		LinkedList child = (LinkedList)getFilhosArvore(groups);
		for (unsigned long i = 0; i < getSizeOfFiguras(child);i++) {
			Arvore next = (Arvore)getListElemAt(child, i);
			drawGroups(next, indice);
		}
		glPopMatrix();
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
	int indice = 0;
	drawGroups(getWorldGroups(world), &indice);
	frames++;
	int time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		float fps = frames * 1000.0f / (time - timebase);
		snprintf(title, 127, "FPS: %.2f, PCAM: (%.2f,%.2f,%.2f)", fps, camx, camy, camz);
		glutSetWindowTitle(title);
		timebase = time;
		frames = 0;
	}

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
	case 'x':
		case 'X':
			showC = !showC;
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
	
	//printWorld(world);
	loadBuffers(getWorldGroups(world), &indice);

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

int countTotalFiguresInTree(Arvore node) {
	int totalFigures = 0;

	// Verifica se o nó é válido
	if (!node) {
		return 0;
	}

	// Conta as figuras no nó atual, se houver
	if (getDataArvore(node)) {
		totalFigures += getSizeOfFiguras(getGroupFigures((Group)getDataArvore(node)));
	}

	// Obtém os filhos do nó
	LinkedList children = getFilhosArvore(node);

	// Percorre recursivamente os filhos do nó
	for (unsigned long i = 0; i < getSizeOfFiguras(children); i++) {
		Arvore child = (Arvore)getListElemAt(children, i);
		totalFigures += countTotalFiguresInTree(child); // Adiciona o número de figuras dos filhos do nó
	}

	return totalFigures;
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


	nrFiguras = countTotalFiguresInTree(getWorldGroups(world));
	//printf("Nr de figuras: %d\n", nrFiguras);

	buffers = (GLuint*)calloc(nrFiguras, sizeof(GLuint));
	timebase = glutGet(GLUT_ELAPSED_TIME);
	tempo_inicial = NOW;
	initGlut(argc, argv, world);

	return 0;
}
