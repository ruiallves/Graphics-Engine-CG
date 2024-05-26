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
		printf("Não foi possível abrir a textura com a diretoria '%s'!\n", texture);
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
			vector<float> toBuffer = figuraToVector(fig); // Converte a figura em um vetor de vértices
			vector<float> toNormals = figuraNormalsToVector(fig); // Converte as normais da figura em um vetor
			const char* textFile = getFiguraTextura(fig); // Obtém o arquivo de textura da figura, se houver
			vector<float> toTextCoors = figuraTexturesToVector(fig, textFile); // Converte as coordenadas de textura da figura em um vetor

			// Vértices
			glBindBuffer(GL_ARRAY_BUFFER, buffers[*index]); // Liga o buffer de vértices
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * toBuffer.size(), toBuffer.data(), GL_STATIC_DRAW); // Carrega os dados dos vértices para o buffer
			sizBuffers.push_back(toBuffer.size() / 3); // Armazena o tamanho do buffer de vértices

			// Normais
			glBindBuffer(GL_ARRAY_BUFFER, buffersN[*index]); // Liga o buffer de normais
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * toNormals.size(), toNormals.data(), GL_STATIC_DRAW); // Carrega os dados das normais para o buffer
			buffersNSizes.push_back(toNormals.size() / 3); // Armazena o tamanho do buffer de normais

			// Texturas
			if (textFile) {
				glBindBuffer(GL_ARRAY_BUFFER, buffersT[*index]); // Liga o buffer de coordenadas de textura
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * toTextCoors.size(), toTextCoors.data(), GL_STATIC_DRAW); // Carrega os dados das coordenadas de textura para o buffer
				cout << "Loading texture: " << textFile << endl; // Exibe a mensagem de carregamento de textura
				loadTexture(textFile, index); // Carrega a textura
			}

			(*index)++; // Incrementa o índice do buffer
			models = (LinkedList)getNext(models); // Avança para o próximo modelo
		}

		LinkedList filhos = getFilhosArvore(groups); // Obtém os filhos do grupo atual
		for (unsigned long i = 0; i < getSizeOfFiguras(filhos); i++) { // Itera sobre os filhos
			Arvore next = (Arvore)getListElemAt(filhos, i); // Obtém o próximo filho
			loadBuffers(next, index); // Carrega os buffers dos filhos recursivamente
		}
	}
}


void drawCatmullRomCurve(const std::vector<std::vector<float>>& controlPoints) {
	if (getWorldLights(world) > 0) glDisable(GL_LIGHTING);
	// Define o número de pontos de amostra ao longo da curva
	const int numSamples = 100;

	// Calcula o intervalo de amostragem
	const float sampleInterval = 1.0f / numSamples;

	// Inicia o desenho da curva como um loop de linha
	glBegin(GL_LINE_LOOP);

	// Itera ao longo da curva e desenha cada ponto de amostra
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
	if (getWorldLights(world) > 0) glEnable(GL_LIGHTING);
}

void drawNormals(Figura model) {
	if (getWorldLights(world) > 0)
		glDisable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 1.0f);
	vector<float> normals = figuraNormalsToVector(model); // Obtém as normais da figura como um vetor de floats
	vector<float> vertexs = figuraToVector(model); // Obtém os vértices da figura como um vetor de floats
	glBegin(GL_LINES);
	for (int i = 0; i < normals.size(); i += 3) { 
		// Desenha uma linha a partir de cada vértice na direção da normal
		glVertex3f(vertexs[i + 0], vertexs[i + 1], vertexs[i + 2]);
		glVertex3f(vertexs[i + 0] + normals[i + 0], vertexs[i + 1] + normals[i + 1], vertexs[i + 2] + normals[i + 2]);
	}
	glEnd(); // Finaliza o desenho das linhas
	if (getWorldLights(world) > 0) // Verifica se há luz no mundo
		glEnable(GL_LIGHTING); // Ativa o cálculo de iluminação novamente
}



void execTransforms(LinkedList transforms, int* indice) {
	LinkedList currentTransform = transforms;

	while (currentTransform != nullptr) {
		Transform transf = (Transform)getData(currentTransform); 
		if (transf != nullptr) { 
			switch (getTransformType(transf)) {
			case 'r': { 
				float angle = getTransformAngle(transf); // Obtém o ângulo da rotação
				float time = getTransformTime(transf); // Obtém o tempo de duração da rotação (se houver)
				if (time != 0) { // Se houver um tempo definido
					// Calcula o ângulo de rotação com base no tempo
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
				float time = getTransformTime(transf); // Obtém o tempo de duração da translação
				if (time > 0) { // Se houver um tempo definido
					float pos[3], deriv[3], y[3], z[3], rot[16];
					// Obtém os pontos de controle para a curva de Catmull-Rom
					vector<vector<float>> points = transPoints(transf);
					// Calcula a posição atual ao longo da curva de Catmull-Rom
					getGlobalCatmullRomPoint(NOW / time, points, pos, deriv);
					// Desenha a curva de Catmull-Rom, se necessário
					if (showC) drawCatmullRomCurve(points);
					// Aplica a translação
					glTranslatef(pos[0], pos[1], pos[2]);

					if (getTransformAlign(transf)) { // Se houver alinhamento
						normalize(deriv); // Normaliza o vetor de derivada
						cross(deriv, transformYAxis(transf).data(), z); // Calcula o vetor Z
						normalize(z); // Normaliza o vetor Z
						cross(z, deriv, y); // Calcula o vetor Y
						setTransformYAxis(transf, y[0], y[1], y[2]); // Define o vetor Y da transformação
						normalize(y); // Normaliza o vetor Y
						buildRotMatrix(deriv, y, z, rot); // Constrói a matriz de rotação
						glMultMatrixf(rot); // Aplica a matriz de rotação
					}
				}
				else { // Se não houver tempo definido
					glTranslatef(getTransformX(transf), getTransformY(transf), getTransformZ(transf));
				}
				break;
			}
			default:
				break;
			}
		}
		currentTransform = (LinkedList)getNext(currentTransform); // Avança para a próxima transformação na lista
	}
}


void drawGroups(Arvore groups, int* indice) {
	if (groups) {
		glPushMatrix(); // Empilha a matriz de modelo-view atual

		Group group = (Group)getDataArvore(groups); // Obtém o grupo de figuras do nó atual
		LinkedList transforms = getGroupTransforms(group), models = getGroupFigures(group); // Obtém as transformações e figuras do grupo

		execTransforms(transforms, indice); // Aplica as transformações ao grupo de figuras

		for (unsigned long i = 0; i < getSizeOfFiguras(models); i++, (*indice)++) {
			Figura fig = (Figura)getListElemAt(models, i); // Obtém uma figura do grupo

			if (fig == nullptr) {
				cerr << "Warning: null figure encountered at index " << i << endl; // Aviso se uma figura for nula
				continue;
			}

			if (showNormais) drawNormals(fig); // Desenha as normais da figura, se ativado

			auto dif = getDif(fig); // Obtém a componente difusa da figura
			if (dif.empty()) {
				cerr << "Error: getDif returned an empty vector for figure at index " << i << endl; // Erro se a componente difusa for vazia
				continue;
			}
			glMaterialfv(GL_FRONT, GL_DIFFUSE, dif.data()); // Define a cor difusa do material da figura

			auto amb = getAmb(fig); // Obtém a componente ambiente da figura
			if (amb.empty()) {
				cerr << "Error: getAmb returned an empty vector for figure at index " << i << endl; // Erro se a componente ambiente for vazia
				continue;
			}
			glMaterialfv(GL_FRONT, GL_AMBIENT, amb.data()); // Define a cor ambiente do material da figura

			auto spec = getSpec(fig); // Obtém a componente especular da figura
			if (spec.empty()) {
				cerr << "Error: getSpec returned an empty vector for figure at index " << i << endl; // Erro se a componente especular for vazia
				continue;
			}
			glMaterialfv(GL_FRONT, GL_SPECULAR, spec.data()); // Define a cor especular do material da figura

			auto emi = getEmi(fig); // Obtém a componente de emissão da figura
			if (emi.empty()) {
				cerr << "Error: getEmi returned an empty vector for figure at index " << i << endl; // Erro se a componente de emissão for vazia
				continue;
			}
			glMaterialfv(GL_FRONT, GL_EMISSION, emi.data()); // Define a cor de emissão do material da figura

			glMaterialf(GL_FRONT, GL_SHININESS, getFiguraShininess(fig)); // Define o brilho do material da figura

			// Texturas
			if (getFiguraTextura(fig)) { // Verifica se a figura tem textura
				glBindTexture(GL_TEXTURE_2D, textures[*indice]); // Liga a textura correspondente
				glBindBuffer(GL_ARRAY_BUFFER, buffersT[*indice]); // Liga o buffer de textura correspondente
				glTexCoordPointer(2, GL_FLOAT, 0, 0); // Define os ponteiros de coordenadas de textura
			}

			// Normais
			glBindBuffer(GL_ARRAY_BUFFER, buffersN[*indice]); // Liga o buffer de normais correspondente
			glNormalPointer(GL_FLOAT, 0, 0); // Define o ponteiro de normais

			// Vértices
			glBindBuffer(GL_ARRAY_BUFFER, buffers[*indice]); // Liga o buffer de vértices correspondente
			glVertexPointer(3, GL_FLOAT, 0, 0); // Define o ponteiro de vértices
			glDrawArrays(GL_TRIANGLES, 0, sizBuffers[*indice]); // Desenha as figuras

			if (getFiguraTextura(fig)) {
				glBindTexture(GL_TEXTURE_2D, 0); // Desliga a textura se houver
			}
		}

		// Procede para fazer o mesmo aos nodos filho.
		LinkedList child = (LinkedList)getFilhosArvore(groups); // Obtém os filhos do nó atual
		for (unsigned long i = 0; i < getSizeOfFiguras(child); i++) {
			Arvore next = (Arvore)getListElemAt(child, i); // Obtém o próximo filho
			drawGroups(next, indice); // Chama recursivamente a função para desenhar os grupos filhos
		}
		glPopMatrix(); // Desempilha a matriz de modelo-view
	}
}



// Função auxiliar para normalizar um vetor
void normalizeVect(float* vec) {
	float length = std::sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
	if (length > 0.0f) {
		vec[0] /= length;
		vec[1] /= length;
		vec[2] /= length;
	}
}

// Função auxiliar para mapear índices de luz
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
		std::cerr << "Erro: Vetor de luzes não inicializado." << std::endl;
		return;
	}

	for (int i = 0; i < lights->size(); i++) {
		Light& luz = (*lights)[i];
		std::vector<float>* lPos = getLightPos(luz);
		std::vector<float>* lDir = getLightDir(luz);

		// Normaliza a direção da luz, se existir
		if (lDir) {
			normalize(lDir->data());
		}

		float cutoff = getLightCutOff(luz);
		GLenum lightId = gl_light(i);
		char lightType = getLightType(luz);

		glEnable(lightId); // Ativa a luz antes de configurá-la

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

	if (getWorldLights(world)->size() > 0) { // Verifica se há luzes no mundo
		glEnable(GL_LIGHTING); // Habilita a iluminação
		glEnable(GL_RESCALE_NORMAL); // Habilita a reescala das normais

		GLfloat white[4] = { 1.0,1.0,1.0,1.0 }; // Cor branca
		for (int i = 0; i < getWorldLights(world)->size(); i++) { // Itera sobre as luzes no mundo
			glEnable(gl_light(i)); // Habilita a luz atual
			glLightfv(gl_light(i), GL_DIFFUSE, white); // Define a cor difusa da luz atual como branca
			glLightfv(gl_light(i), GL_SPECULAR, white); // Define a cor especular da luz atual como branca
		}

		float amb[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Cor ambiente
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb); // Define o modelo de cor ambiente
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
	buffersN = (GLuint*)calloc(nrFiguras, sizeof(GLuint));
	buffersT = (GLuint*)calloc(nrFiguras, sizeof(GLuint));
	textures = (GLuint*)calloc(nrFiguras, sizeof(GLuint));

	timebase = glutGet(GLUT_ELAPSED_TIME);
	tempo_inicial = NOW;
	initGlut(argc, argv, world);

	return 0;
}
