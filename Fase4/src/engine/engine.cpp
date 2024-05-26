#define _NO_CRT_STDIO_INLINE

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <../devil/IL/il.h>
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
vector<int> sizBuffers;
GLuint* buffers = NULL;

GLuint* buffersN = NULL;
vector<unsigned int> buffersNSizes;
GLuint* buffersT = NULL;
GLuint* textures = NULL;

char title[128];
int frames = 0;
int timebase = 0;
float tempo_inicial = 0.0f;

bool showC = false;

bool showNormais = false;

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
	if (getWorldLights(world) > 0) glDisable(GL_LIGHTING);
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
	if (getWorldLights(world) > 0) glEnable(GL_LIGHTING);
}

void loadTexture(const char* texturePath, int* index) {
	// Construa o caminho completo para a textura
	string path = "../../../textures/solar/";
	path += texturePath;
	const char* texture = path.c_str();

	// Abra o arquivo de textura para verificar se ele existe
	FILE* file_ptr = fopen(texture, "r");
	if (file_ptr == NULL) {
		printf("N�o foi poss�vel abrir a textura com a diretoria '%s'!\n", texture);
		exit(1);
	}
	fclose(file_ptr);
	unsigned int t, tw, th;
	unsigned char* texData;
	ilGenImages(1, &t);
	ilBindImage(t);
	ilLoadImage((ILstring)texture);
	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	texData = ilGetData();

	glGenTextures(1, textures + *index);

	glBindTexture(GL_TEXTURE_2D, *(textures + *index));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void loadBuffers(Arvore groups, int* index) {
	if (groups) {
		Group group = (Group)getDataArvore(groups);
		LinkedList models = getGroupFigures(group);

		while (models != nullptr) {
			Figura fig = (Figura)getData(models);
			vector<float> toBuffer = figuraToVector(fig);
			vector<float> toNormals = figuraNormalsToVector(fig);
			const char* textFile = getFiguraTextura(fig);
			vector<float> toTextCoors = figuraTexturesToVector(fig, textFile);

			// V�rtices
			glBindBuffer(GL_ARRAY_BUFFER, buffers[*index]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * toBuffer.size(), toBuffer.data(), GL_STATIC_DRAW);
			sizBuffers.push_back(toBuffer.size() / 3);

			// Normais
			glBindBuffer(GL_ARRAY_BUFFER, buffersN[*index]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * toNormals.size(), toNormals.data(), GL_STATIC_DRAW);
			buffersNSizes.push_back(toNormals.size() / 3);

			// Texturas
			if (textFile) {
				glBindBuffer(GL_ARRAY_BUFFER, buffersT[*index]);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * toTextCoors.size(), toTextCoors.data(), GL_STATIC_DRAW);
				cout << "Loading texture: " << textFile << endl;
				loadTexture(textFile, index);
			}

			(*index)++;
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
	if (getWorldLights(world) > 0) glDisable(GL_LIGHTING);
	// Define o n�mero de pontos de amostra ao longo da curva
	const int numSamples = 100;

	// Calcula o intervalo de amostragem
	const float sampleInterval = 1.0f / numSamples;

	// Inicia o desenho da curva como um loop de linha
	glBegin(GL_LINE_LOOP);

	// Itera ao longo da curva e desenha cada ponto de amostra
	for (int i = 0; i <= numSamples; ++i) {
		// Calcula o par�metro t para este ponto de amostra
		float t = i * sampleInterval;

		// Calcula as coordenadas (x, y, z) do ponto de amostra usando a fun��o getGlobalCatmullRomPoint
		float pos[3];
		float deriv[3];
		getGlobalCatmullRomPoint(t, controlPoints, pos, deriv);

		// Desenha o ponto de amostra
		glVertex3f(pos[0], pos[1], pos[2]);
	}

	// Finalizando o desenho da curva
	glEnd();
	if (getWorldLights(world) > 0) glEnable(GL_LIGHTING);
}

void drawNormals(Figura model) {
	if (getWorldLights(world) > 0) glDisable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 1.0f);
	vector<float> normals = figuraNormalsToVector(model);
	vector<float> vertexs = figuraToVector(model);
	glBegin(GL_LINES);
	for (int i = 0; i < normals.size(); i += 3) {
		glVertex3f(vertexs[i + 0], vertexs[i + 1], vertexs[i + 2]);
		glVertex3f(vertexs[i + 0] + normals[i + 0], vertexs[i + 1] + normals[i + 1], vertexs[i + 2] + normals[i + 2]);
	}
	glEnd();
	if (getWorldLights(world) > 0) glEnable(GL_LIGHTING);
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
				float time = getTransformTime(transf);
				if (time > 0) {
					float pos[3], deriv[3], y[3], z[3], rot[16];
					vector<vector<float>> points = transPoints(transf);
					getGlobalCatmullRomPoint(NOW / time, points, pos, deriv);
					if (showC) drawCatmullRomCurve(points);
					glTranslatef(pos[0], pos[1], pos[2]);

					if (getTransformAlign(transf)) {
						normalize(deriv);
						cross(deriv, transformYAxis(transf).data(), z);
						normalize(z);
						cross(z, deriv, y);
						setTransformYAxis(transf, y[0], y[1], y[2]);
						normalize(y);
						buildRotMatrix(deriv, y, z, rot);
						glMultMatrixf(rot);
					}
				}
				else {
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
			Figura fig = (Figura)getListElemAt(models, i);

			if (fig == nullptr) {
				cerr << "Warning: null figure encountered at index " << i << endl;
				continue;
			}

			if (showNormais) drawNormals(fig);

			auto dif = getDif(fig);
			if (dif.empty()) {
				cerr << "Error: getDif returned an empty vector for figure at index " << i << endl;
				continue;
			}
			glMaterialfv(GL_FRONT, GL_DIFFUSE, dif.data());

			auto amb = getAmb(fig);
			if (amb.empty()) {
				cerr << "Error: getAmb returned an empty vector for figure at index " << i << endl;
				continue;
			}
			glMaterialfv(GL_FRONT, GL_AMBIENT, amb.data());

			auto spec = getSpec(fig);
			if (spec.empty()) {
				cerr << "Error: getSpec returned an empty vector for figure at index " << i << endl;
				continue;
			}
			glMaterialfv(GL_FRONT, GL_SPECULAR, spec.data());

			auto emi = getEmi(fig);
			if (emi.empty()) {
				cerr << "Error: getEmi returned an empty vector for figure at index " << i << endl;
				continue;
			}
			glMaterialfv(GL_FRONT, GL_EMISSION, emi.data());

			glMaterialf(GL_FRONT, GL_SHININESS, getFiguraShininess(fig));

			// Texturas
			if (getFiguraTextura(fig)) {
				glBindTexture(GL_TEXTURE_2D, textures[*indice]);
				glBindBuffer(GL_ARRAY_BUFFER, buffersT[*indice]);
				glTexCoordPointer(2, GL_FLOAT, 0, 0);
			}

			// Normais
			glBindBuffer(GL_ARRAY_BUFFER, buffersN[*indice]);
			glNormalPointer(GL_FLOAT, 0, 0);

			// Vertices
			glBindBuffer(GL_ARRAY_BUFFER, buffers[*indice]);
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glDrawArrays(GL_TRIANGLES, 0, sizBuffers[*indice]);

			if (getFiguraTextura(fig)) {
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}

		// Procede para fazer o mesmo aos nodos filho.
		LinkedList child = (LinkedList)getFilhosArvore(groups);
		for (unsigned long i = 0; i < getSizeOfFiguras(child); i++) {
			Arvore next = (Arvore)getListElemAt(child, i);
			drawGroups(next, indice);
		}
		glPopMatrix();
	}
}


// Fun��o auxiliar para normalizar um vetor
void normalizeVect(float* vec) {
	float length = std::sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
	if (length > 0.0f) {
		vec[0] /= length;
		vec[1] /= length;
		vec[2] /= length;
	}
}

// Fun��o auxiliar para mapear �ndices de luz
GLenum gl_light(int index) {
	switch (index) {
	case 0: return GL_LIGHT0;
	case 1: return GL_LIGHT1;
	case 2: return GL_LIGHT2;
	case 3: return GL_LIGHT3;
	case 4: return GL_LIGHT4;
	case 5: return GL_LIGHT5;
	case 6: return GL_LIGHT6;
	case 7: return GL_LIGHT7;
	default: return GL_LIGHT0; // Default case
	}
}


void execLights(std::vector<Light>* lights) {
	if (!lights) {
		std::cerr << "Erro: Vetor de luzes n�o inicializado." << std::endl;
		return;
	}

	for (int i = 0; i < lights->size(); i++) {
		Light& luz = (*lights)[i];
		std::vector<float>* lPos = getLightPos(luz);
		std::vector<float>* lDir = getLightDir(luz);

		// Normaliza a dire��o da luz, se existir
		if (lDir) {
			normalize(lDir->data());
		}

		float cutoff = getLightCutOff(luz);
		GLenum lightId = gl_light(i);
		char lightType = getLightType(luz);

		glEnable(lightId); // Ativa a luz antes de configur�-la

		if (lightType == 'p' && lPos) { // Point light
			std::vector<float> pos = *lPos;
			pos.push_back(1.0f); // Adiciona 1.0 para a componente w
			glLightfv(lightId, GL_POSITION, pos.data());
		}
		else if (lightType == 'd' && lDir) { // Directional light
			std::vector<float> dir = *lDir;
			dir.push_back(0.0f); // Adiciona 0.0 para a componente w
			glLightfv(lightId, GL_POSITION, dir.data());
		}
		else if (lightType == 's') { // Spotlight
			if (lPos) {
				std::vector<float> pos = *lPos;
				pos.push_back(1.0f); // Adiciona 1.0 para a componente w
				glLightfv(lightId, GL_POSITION, pos.data());
			}
			if (lDir) {
				glLightfv(lightId, GL_SPOT_DIRECTION, lDir->data());
			}
			glLightf(lightId, GL_SPOT_CUTOFF, cutoff);
			glLightf(lightId, GL_SPOT_EXPONENT, 0.0f);
		}
		else {
			std::cerr << "Tipo de luz desconhecido: " << lightType << std::endl;
		}
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
	execLights(getWorldLights(world));
	//printWorld(world);
	int indice = 0; // buffer lido
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

	case 'n':
		case 'N':
			showNormais = !showNormais;
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
	glutCreateWindow("Fase 4");

	// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

	glewInit();
	ilInit();
	// put here the registration of the keyboard callbacks
	glutKeyboardFunc(keyboardFunc);
	glutSpecialFunc(keyboardspecial);

	if (getWorldLights(world)->size() > 0) {
		glEnable(GL_LIGHTING);
		glEnable(GL_RESCALE_NORMAL);

		GLfloat white[4] = { 1.0,1.0,1.0,1.0 };
		for (int i = 0; i < getWorldLights(world)->size(); i++) {
			glEnable(gl_light(i));
			glLightfv(gl_light(i), GL_DIFFUSE, white);
			glLightfv(gl_light(i), GL_SPECULAR, white);
		}

		float amb[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

	}


	//  OpenGL settings
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glPolygonMode(GL_FRONT, GL_LINE);
	glGenBuffers(nrFiguras, buffers);
	glGenBuffers(nrFiguras, buffersN);
	glGenBuffers(nrFiguras, buffersT);

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

	// Verifica se o n� � v�lido
	if (!node) {
		return 0;
	}

	// Conta as figuras no n� atual, se houver
	if (getDataArvore(node)) {
		totalFigures += getSizeOfFiguras(getGroupFigures((Group)getDataArvore(node)));
	}

	// Obt�m os filhos do n�
	LinkedList children = getFilhosArvore(node);

	// Percorre recursivamente os filhos do n�
	for (unsigned long i = 0; i < getSizeOfFiguras(children); i++) {
		Arvore child = (Arvore)getListElemAt(children, i);
		totalFigures += countTotalFiguresInTree(child); // Adiciona o n�mero de figuras dos filhos do n�
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
	buffers = (GLuint*)calloc(nrFiguras, sizeof(GLuint)); // para cada figura
	buffersN = (GLuint*)calloc(nrFiguras, sizeof(GLuint)); // para cada normal
	buffersT = (GLuint*)calloc(nrFiguras, sizeof(GLuint));
	textures = (GLuint*)calloc(nrFiguras, sizeof(GLuint));

	timebase = glutGet(GLUT_ELAPSED_TIME);
	tempo_inicial = NOW;
	initGlut(argc, argv, world);

	return 0;
}
