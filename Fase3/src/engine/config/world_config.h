#ifndef WORLD_CONFIG
#define WORLD_CONFIG

#include "../../tinyxml/tinyxml.h"
#include "../../tools/linkedList.h"
#include "../../tools/figura.h"
#include "../../tools/arvore.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <stdlib.h>

typedef struct window* Window;
typedef struct projection* Projection;
typedef struct camera* Camera;
typedef struct transform* Transform;
typedef struct world* World;
typedef struct group* Group;

Group newGroup();
World parseXmlFile(World* world, const char* filePath);
World newConfig();

// Funções de criação
Group newGroup();
Transform newTransform(char type, float x, float y, float z, float angle, float time, vector<vector<float>> point, bool align);
World newConfig();

// Getters e Setters para a estrutura Window
int getWindowWidth(Window window);
void setWindowWidth(Window window, int width);
int getWindowHeight(Window window);
void setWindowHeight(Window window, int height);

// Getters e Setters para a estrutura Projection
float getProjectionFOV(Projection projection);
void setProjectionFOV(Projection projection, float fov);
float getProjectionNear(Projection projection);
void setProjectionNear(Projection projection, float near);
float getProjectionFar(Projection projection);
void setProjectionFar(Projection projection, float far);

// Getters e Setters para a estrutura Camera
float* getCameraPosition(Camera camera);
void setCameraPosition(Camera camera, float x, float y, float z);
float* getCameraLookAt(Camera camera);
void setCameraLookAt(Camera camera, float x, float y, float z);
float* getCameraUp(Camera camera);
void setCameraUp(Camera camera, float x, float y, float z);
Projection getCameraProjection(Camera camera);
void setCameraProjection(Camera camera, Projection projection);

// Getters e Setters para a estrutura World
Window getWorldWindow(World world);
void setWorldWindow(World world, Window window);
Camera getWorldCamera(World world);
void setWorldCamera(World world, Camera camera);
Arvore getWorldGroups(World world);
void setWorldGroups(World world, Arvore grupos);

// Getters e Setters para a estrutura Group
LinkedList getGroupTransforms(Group group);
LinkedList getGroupFigures(Group group);

// Getters e Setters para a estrutura Transform
char getTransformType(Transform transform);
void setTransformType(Transform transform, char tipo);
float getTransformX(Transform transform);
void setTransformX(Transform transform, float x);
float getTransformY(Transform transform);
void setTransformY(Transform transform, float y);
float getTransformZ(Transform transform);
void setTransformZ(Transform transform, float z);
float getTransformAngle(Transform transform);
void setTransformAngle(Transform transform, float angle);
float getTransformTime(Transform transform);
void setTransformTime(Transform transform, float time);
vector<vector<float>>* getTransformPoints(Transform transform);
void setTransformPoints(Transform transform, vector<vector<float>>* points);
bool getTransformAlign(Transform transform);
void setTransformAlign(Transform transform, bool align);
vector<vector<float>> transPoints(Transform transf);
vector<float> transformYAxis(Transform transf);
void setTransformYAxis(Transform transform, float x, float y, float z);

// Funções auxiliares
Window getWindow(World world);
void setWindow(World* world, int width, int height);
Camera getCamera(World world);
Projection getProjection(Camera camera);
void setProjection(Projection projection, float fov, float near, float far);

void printGroup(Group group);
void printWorld(World world);

#endif