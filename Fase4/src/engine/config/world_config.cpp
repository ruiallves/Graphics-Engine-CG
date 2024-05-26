#include "world_config.h"

struct window {
    int width;
    int height;
};

struct projection {
    float fov;
    float near;
    float far;
};

struct camera {
    float position[3];
    float lookAt[3];
    float up[3];
    Projection projection;
};

struct world {
    Window window;
    Camera camera;
    Arvore grupos;
    vector<Light>* lights;
};

struct group {
    LinkedList transformacoes;
    LinkedList figuras;
};

struct transform {
    char tipo;
    float x, y, z;
    float anguloR;
    float time;
    bool align;
    vector<vector<float>>* points;
    float yAxis[3];
};

struct light {
    char tipo;
    vector<float>* pos;
    vector<float>* dir;
    float cutOff;
};

Group newGroup() {
    Group ng = (Group)malloc(sizeof(struct group));
    if (ng) {
        ng->figuras = newLinkedListEmpty();
        ng->transformacoes = newLinkedListEmpty();
    }
    return ng;
}

Window newWindow() {
    Window wd = (Window)malloc(sizeof(struct window));
    return wd;
}

Projection newProjection() {
    Projection pj = (Projection)malloc(sizeof(struct projection));
    return pj;
}

Camera newCamera() {
    Camera cam = (Camera)malloc(sizeof(struct camera));
    return cam;
}

Light newEmptyLight() {
    Light light = (Light)malloc(sizeof(struct light));
    if (light)
    {
        light->tipo = 'n';

        light->pos = new vector<float>();
        light->pos->push_back(0.0f);
        light->pos->push_back(0.0f);
        light->pos->push_back(0.0f);
        light->pos->push_back(1.0f);

        light->dir = new vector<float>();
        light->dir->push_back(0.0f);
        light->dir->push_back(0.0f);
        light->dir->push_back(0.0f);
        light->dir->push_back(0.0f);

        light->cutOff = 0.0f;
    }

    return light;
}

Light newLight(char type, float x, float y, float z, float dx, float dy, float dz, float cutOff) {
    Light light = (Light)malloc(sizeof(struct light));
    if (light)
    {
        light->tipo = type;

        light->pos = new vector<float>();
        light->pos->push_back(x);
        light->pos->push_back(y);
        light->pos->push_back(z);
        light->pos->push_back(1.0f);

        light->dir = new vector<float>();
        light->dir->push_back(dx);
        light->dir->push_back(dy);
        light->dir->push_back(dz);
        light->dir->push_back(0.0f);

        light->cutOff = cutOff;
    }

    return light;
}

Light newLightVec(char type, float* pos, float* dir, float cutOff) {
    Light light = (Light)malloc(sizeof(struct light));
    if (light)
    {
        light->tipo = type;

        light->pos = new vector<float>();
        light->pos->push_back(pos[0]);
        light->pos->push_back(pos[1]);
        light->pos->push_back(pos[2]);

        light->dir = new vector<float>();
        light->dir->push_back(dir[0]);
        light->dir->push_back(dir[1]);
        light->dir->push_back(dir[2]);

        light->cutOff = cutOff;
    }

    return light;
}

Transform newTransform(char type, float x, float y, float z, float angle = 0.0f, float time = 0.0f, vector<vector<float>> points = {}, bool align = false) {
    Transform transform = (Transform)malloc(sizeof(struct transform));
    transform->points = new vector<vector<float>>();
    if (transform) {
        transform->tipo = type;
        transform->x = x;
        transform->y = y;
        transform->z = z;
        transform->anguloR = angle;
        transform->time = time;
        for (vector<float> point : points) {
            vector<float> ptn;
            ptn.push_back(point[0]);
            ptn.push_back(point[1]);
            ptn.push_back(point[2]);
            transform->points->push_back(ptn);
        }
        transform->align = align;
        transform->yAxis[0] = 0.0f;
        transform->yAxis[1] = 1.0f;
        transform->yAxis[2] = 0.0f;
    }
    return transform;
}

World newConfig() {
    World newConf = (World)malloc(sizeof(struct world));
    if (newConf) {
        newConf->camera = newCamera();
        newConf->camera->projection = newProjection();
        newConf->grupos = newArvoreEmpty();
        newConf->lights = new vector<Light>();
    }
    return newConf;
}

// Getters e Setters para a estrutura Window
int getWindowWidth(Window window) {
    return window->width;
}

void setWindowWidth(Window window, int width) {
    window->width = width;
}

int getWindowHeight(Window window) {
    return window->height;
}

void setWindowHeight(Window window, int height) {
    window->height = height;
}

// Getters e Setters para a estrutura Projection
float getProjectionFOV(Projection projection) {
    return projection->fov;
}

void setProjectionFOV(Projection projection, float fov) {
    projection->fov = fov;
}

float getProjectionNear(Projection projection) {
    return projection->near;
}

void setProjectionNear(Projection projection, float near) {
    projection->near = near;
}

float getProjectionFar(Projection projection) {
    return projection->far;
}

void setProjectionFar(Projection projection, float far) {
    projection->far = far;
}

// Getters e Setters para a estrutura Camera
float* getCameraPosition(Camera camera) {
    return camera->position;
}

void setCameraPosition(Camera camera, float x, float y, float z) {
    camera->position[0] = x;
    camera->position[1] = y;
    camera->position[2] = z;
}

float* getCameraLookAt(Camera camera) {
    return camera->lookAt;
}

void setCameraLookAt(Camera camera, float x, float y, float z) {
    camera->lookAt[0] = x;
    camera->lookAt[1] = y;
    camera->lookAt[2] = z;
}

float* getCameraUp(Camera camera) {
    return camera->up;
}

void setCameraUp(Camera camera, float x, float y, float z) {
    camera->up[0] = x;
    camera->up[1] = y;
    camera->up[2] = z;
}

Projection getCameraProjection(Camera camera) {
    return camera->projection;
}

void setCameraProjection(Camera camera, Projection projection) {
    camera->projection = projection;
}

// Getters e Setters para a estrutura World
Window getWorldWindow(World world) {
    return world->window;
}

void setWorldWindow(World world, Window window) {
    world->window = window;
}

Camera getWorldCamera(World world) {
    return world->camera;
}

void setWorldCamera(World world, Camera camera) {
    world->camera = camera;
}

Arvore getWorldGroups(World world) {
    if (world) {
        return world->grupos;
    }
    return NULL;
}

void setWorldGroups(World world, Arvore grupos) {
    world->grupos = grupos;
}

vector<Light>* getWorldLights(World world) {
    return world->lights;
}

void setWorldLights(World world, vector<Light>* lights) {
    world->lights = lights;
}

// Getters e Setters para a estrutura Group
LinkedList getGroupTransforms(Group group) {
    return group->transformacoes;
}

LinkedList getGroupFigures(Group group) {
    return group->figuras;
}

void setGroupTransforms(Group group, LinkedList transformacoes) {
    group->transformacoes = transformacoes;
}

void setGroupFigures(Group group, LinkedList figuras) {
    group->figuras = figuras;
}

// Getters e Setters para a estrutura Transform
char getTransformType(Transform transform) {
    return transform->tipo;
}

void setTransformType(Transform transform, char tipo) {
    transform->tipo = tipo;
}

float getTransformX(Transform transform) {
    return transform->x;
}

void setTransformX(Transform transform, float x) {
    transform->x = x;
}

float getTransformY(Transform transform) {
    return transform->y;
}

void setTransformY(Transform transform, float y) {
    transform->y = y;
}

float getTransformZ(Transform transform) {
    return transform->z;
}

void setTransformZ(Transform transform, float z) {
    transform->z = z;
}

float getTransformAngle(Transform transform) {
    return transform->anguloR;
}

void setTransformAngle(Transform transform, float angle) {
    transform->anguloR = angle;
}

float getTransformTime(Transform transform) {
    return transform->time;
}

void setTransformTime(Transform transform, float time) {
    transform->time = time;
}

bool getTransformAlign(Transform transform) {
    return transform->align;
}

void setTransformAlign(Transform transform, bool align) {
    transform->align = align;
}

vector<vector<float>>* getTransformPoints(Transform transform) {
    return transform->points;
}

void setTransformPoints(Transform transform, vector<vector<float>>* points) {
    transform->points = points;
}

vector<vector<float>> transPoints(Transform transf) {
    vector<vector<float>> result;
    if (transf) {
        for (vector<float> point : *(transf->points)) {
            result.push_back(point);
        }
    }
    return result;
}

vector<float> transformYAxis(Transform transf) {
    vector<float> result;
    if (transf) {
        result.push_back(transf->yAxis[0]);
        result.push_back(transf->yAxis[1]);
        result.push_back(transf->yAxis[2]);
        return result;
    }
    return result;
}

void setTransformYAxis(Transform transform, float x, float y, float z) {
    transform->yAxis[0] = x;
    transform->yAxis[1] = y;
    transform->yAxis[2] = z;
}

Window getWindow(World world) {
    return world->window;
}

void setWindow(World* world, int width, int height) {
    Window w = newWindow();
    setWindowHeight(w, height);
    setWindowWidth(w, width);
    setWorldWindow(*world, w);
}

Camera getCamera(World world) {
    return world->camera;
}

Projection getProjection(Camera camera) {
    return camera->projection;
}

void setProjection(Projection projection, float fov, float near, float far) {
    projection->fov = fov;
    projection->near = near;
    projection->far = far;
}

//getters light

char getLightType(Light light) {
	return light->tipo;
}

vector<float>* getLightPos(Light light) {
	return light->pos;
}

vector<float>* getLightDir(Light light) {
	return light->dir;
}

float getLightCutOff(Light light) {
	return light->cutOff;
}

void printTransforms(LinkedList transforms) {
    LinkedList current = transforms;

    // Percorre a lista e imprime os valores de cada transformação
    while (current != NULL) {
        Transform transform = (Transform)getData(current);
        if (transform != NULL) {
            if (transform->tipo == 't') {
                printf("Time: %.2f, Align: %s\n", transform->time, transform->align ? "true" : "false");
                printf("Pontos:\n");
                for (const auto& point : *(transform->points)) {
                    printf("(%.2f, %.2f, %.2f)\n", point[0], point[1], point[2]);
                }
                printf("Eixo Y: (%.2f, %.2f, %.2f)\n", transform->yAxis[0], transform->yAxis[1], transform->yAxis[2]);
            }
            else {
                printf("Tipo: %c, Posição: (%.2f, %.2f, %.2f), Ângulo: %.2f\n",
                    transform->tipo, transform->x, transform->y, transform->z, transform->anguloR);
            }
        }
        current = (LinkedList)getNext(current);
    }

}

void printLinkedList(LinkedList list) {
    LinkedList current = list;
    while (current != NULL) {
        float value = *(float*)getData(current);
        printf("%.2f ", value);
        current = (LinkedList)getNext(current);
    }
    printf("\n");
}

void printFiguras(LinkedList figuras) {
    LinkedList current = figuras;

    // Percorre a lista de figuras
    while (current != NULL) {
        Figura fig = (Figura)getData(current);
        if (fig != NULL) {
            printf("Figura com %lu vertices\n", getTotalVertices(fig));
            printf("Textura: %s\n", getFiguraTextura(fig) ? getFiguraTextura(fig) : "N/A");

            // Print vertices
            printf("Vertices: ");
            printLinkedList(getFiguraVertices(fig));

            // Print normais
            printf("Normais: ");
            printLinkedList(getFiguraNormais(fig));

            // Print textCoordenadas
            printf("Coordenadas de Textura: ");
            printLinkedList(getFiguraTextCoordenadas(fig));

            // Print diffuse color
            LinkedList figDif = getFiguraDiffuse(fig);
            if (figDif != NULL) {
                float r = *(float*)getData(figDif);
                LinkedList next = (LinkedList)getNext(figDif);
                if (next != NULL) {
                    float g = *(float*)getData(next);
                    LinkedList nextNext = (LinkedList)getNext(next);
                    if (nextNext != NULL) {
                        float b = *(float*)getData(nextNext);
                        LinkedList nextNextNext = (LinkedList)getNext(nextNext);
                        if (nextNextNext != NULL) {
							float a = *(float*)getData(nextNextNext);
							printf("Cor difusa: %.2f, %.2f, %.2f, %.2f\n", r, g, b, a);
                        }
                    }

                    else {
                        printf("Erro: Não foi possível obter o valor de 'b'.\n");
                    }
                }
                else {
                    printf("Erro: Não foi possível obter o valor de 'g'.\n");
                }
            }
            else {
                printf("Erro: Não foi possível obter os dados de difusão.\n");
            }

            // Print ambient color
            LinkedList figAmb = getFiguraAmbient(fig);
            if (figAmb != NULL) {
                printf("Cor ambiente: ");
                printLinkedList(figAmb);
            }
            else {
                printf("Erro: Não foi possível obter os dados de ambiente.\n");
            }

            // Print specular color
            LinkedList figSpec = getFiguraSpecular(fig);
            if (figSpec != NULL) {
                printf("Cor especular: ");
                printLinkedList(figSpec);
            }
            else {
                printf("Erro: Não foi possível obter os dados de especular.\n");
            }

            // Print emissive color
            LinkedList figEmi = getFiguraSpecular(fig);
            if (figEmi != NULL) {
                printf("Cor emissiva: ");
                printLinkedList(figEmi);
            }
            else {
                printf("Erro: Não foi possível obter os dados de emissiva.\n");
            }

            // Print shininess
            printf("Brilho: %.2f\n", getFiguraShininess(fig));
        }
        current = (LinkedList)getNext(current);
    }
}


void addTransform(Arvore ll, Transform transform) {
    if (ll && transform) {
        Group group = (Group)getDataArvore(ll);
        LinkedList transforms = getGroupTransforms(group);
        addChild(&transforms, transform);
        setGroupTransforms(group, transforms);
    }
}


void addModel(Arvore ll, Figura figura) {
    if (ll && figura) {
        Group group = (Group)getDataArvore(ll);
        LinkedList figuras = getGroupFigures(group);
        addChild(&figuras, figura);
        setGroupFigures(group, figuras);
    }
}

void printGroup(Group group) {
    if (group == nullptr) {
        std::cerr << "Grupo não inicializado." << std::endl;
        return;
    }

    std::cout << "Transformações do grupo:" << std::endl;
    printTransforms(getGroupTransforms(group));

    std::cout << "Figuras do grupo:" << std::endl;
    printFiguras(getGroupFigures(group));
}



Arvore parseGroups(TiXmlElement* group) {
    if (group) {
        Arvore res = newArvore(newGroup());
        Transform transform_obj = NULL;

        TiXmlElement* transforms = group->FirstChildElement("transform");

        if (transforms) {
            for (TiXmlElement* t = transforms->FirstChildElement(); t; t = t->NextSiblingElement()) {
                vector<vector<float>> points;
                const char* name_of_transform = t->Value();
                //printf("name_of_transform: %s\n", name_of_transform);   
                float angle = 0.0f, time = 0.0f;
                bool align = false;
                if (strcmp(name_of_transform, "rotate") == 0) {
                    angle = t->Attribute("angle") ? atof(t->Attribute("angle")) : 0.0f;
                    time = t->Attribute("time") ? atof(t->Attribute("time")) : 0.0f;
                    //printf("angle: %f\n", angle);
                    //printf("time: %f\n", time);
                }
                else if (strcmp(name_of_transform, "translate") == 0) {
                    if (t->Attribute("time") || t->Attribute("align")) {
                        time = t->Attribute("time") ? atof(t->Attribute("time")) : 0.0f;
                        align = t->Attribute("align") ? (strcmp(t->Attribute("align"), "true") == 0 ? true : false) : false;
                        //printf("time: %f\n", time);
                        //printf("align: %d\n", align);
                        for (TiXmlElement* p = t->FirstChildElement("point"); p; p = p->NextSiblingElement()) {
                            vector<float> ptn;
                            ptn.push_back(atof(p->Attribute("x")));
                            ptn.push_back(atof(p->Attribute("y")));
                            ptn.push_back(atof(p->Attribute("z")));
                            //printf("point: (%f, %f, %f)\n", ptn[0], ptn[1], ptn[2]);
                            points.push_back(ptn);
                        }
                    }
                }
                float x = t->Attribute("x") ? atof(t->Attribute("x")) : 0.0f;
                float y = t->Attribute("y") ? atof(t->Attribute("y")) : 0.0f;
                float z = t->Attribute("z") ? atof(t->Attribute("z")) : 0.0f;
                //printf("x: %f\n", x);
                //printf("y: %f\n", y);
                //printf("z: %f\n", z);

                Transform trans = newTransform(name_of_transform[0], x, y, z, angle, time, points, align);
                addTransform(res, trans);
            }
        }

        //A FUNCIONAR
        //Group groupt = (Group)getDataArvore(res);
        //LinkedList transf = getGroupTransforms(groupt);
        //printTransforms(transf);

        // METER OS MODELS NA ARVORE
        TiXmlElement* models = group->FirstChildElement("models");
        if (models) {
            for (TiXmlElement* m = models->FirstChildElement("model"); m; m = m->NextSiblingElement("model")) {
                Figura fig = newFigura();
                const char* file_name = m->Attribute("file");
                fig = fileToFigura(file_name);

                TiXmlElement* color = m->FirstChildElement("color"), * diffuse = NULL, * ambient = NULL, * specular = NULL, * emissive = NULL, * shininess = NULL;
                TiXmlElement* texture = m->FirstChildElement("texture");

                if (color) {
					diffuse = color->FirstChildElement("diffuse");
					ambient = color->FirstChildElement("ambient");
					specular = color->FirstChildElement("specular");
					emissive = color->FirstChildElement("emissive");
					shininess = color->FirstChildElement("shininess");
				}

                if (texture) {
                    setFiguraTextura(fig, texture->Attribute("file"));
                }

                if (diffuse) {
                    LinkedList dif = newLinkedListEmpty();
                    if (dif != NULL) {
                        float* r = new float(ATOF(diffuse->Attribute("R")));
                        float* g = new float(ATOF(diffuse->Attribute("G")));
                        float* b = new float(ATOF(diffuse->Attribute("B")));
                        float* a = new float(1.0f);
                        if (r != NULL && g != NULL && b != NULL) {
                            addChild(&dif, static_cast<void*>(r));
                            addChild(&dif, static_cast<void*>(g));
                            addChild(&dif, static_cast<void*>(b));
                            addChild(&dif, static_cast<void*>(a));
                            setFiguraDiffuse(fig, dif);
                            //printf("Diffuse color: (%f, %f, %f) added to LinkedList\n", *r, *g, *b);
                        }
                        else {
                            printf("Erro: Falha na alocação de memória para valores de difusão.\n");
                        }
                    }
                    else {
                        printf("Erro: Falha na criação da lista de difusão.\n");
                    }
                }

                if (ambient) {
                    LinkedList amb = newLinkedListEmpty();
					addChild(&amb, static_cast<void*>(new float(ATOF(ambient->Attribute("R")))));
					addChild(&amb, static_cast<void*>(new float(ATOF(ambient->Attribute("G")))));
					addChild(&amb, static_cast<void*>(new float(ATOF(ambient->Attribute("B")))));
                    addChild(&amb, static_cast<void*>(new float(1.0f)));
					setFiguraAmbient(fig, amb);
                }

                if (specular) {
					LinkedList spec = newLinkedListEmpty();
                    addChild(&spec, static_cast<void*>(new float(ATOF(specular->Attribute("R")))));
                    addChild(&spec, static_cast<void*>(new float(ATOF(specular->Attribute("G")))));
                    addChild(&spec, static_cast<void*>(new float(ATOF(specular->Attribute("B")))));
                    addChild(&spec, static_cast<void*>(new float(1.0f)));
                    setFiguraSpecular(fig, spec);
                }

                if (emissive) {
                    LinkedList emi = newLinkedListEmpty();
                    addChild(&emi, static_cast<void*>(new float(ATOF(emissive->Attribute("R")))));
                    addChild(&emi, static_cast<void*>(new float(ATOF(emissive->Attribute("G")))));
                    addChild(&emi, static_cast<void*>(new float(ATOF(emissive->Attribute("B")))));
                    addChild(&emi, static_cast<void*>(new float(1.0f)));
                    setFiguraEmissive(fig, emi);
                }

                if (shininess) {
					setFiguraShininess(fig, ATOF(shininess->Attribute("value")));
				}

                addModel(res, fig);
            }
        }


        //Group groupt = (Group)getDataArvore(res);
        //LinkedList figs = getGroupFigures(groupt);
        //printFiguras(figs);

        for (TiXmlElement* chGroup = group->FirstChildElement("group"); chGroup; chGroup = chGroup->NextSiblingElement("group")) {
            Arvore child = parseGroups(chGroup);
            appendArvore(res, child);
        }
        return res;
    }
    return NULL;
}

vector<Light>* parseLights(TiXmlElement* lights) {
    if (lights == nullptr) return nullptr; // Se não houver dados de iluminação, não faz nada.

    vector<Light>* lightsVec = new vector<Light>();
    // Itera sobre todos os elementos "light".
    for (TiXmlElement* lightElem = lights->FirstChildElement("light"); lightElem != nullptr; lightElem = lightElem->NextSiblingElement("light")) {
        Light newLight = newEmptyLight();
        newLight->tipo = lightElem->Attribute("type")[0]; // Assume que o atributo "type" existe e tem pelo menos um caractere.

        // Inicializa as posições e direções com zero.
        std::vector<float> pos = { 0.0f, 0.0f, 0.0f };
        std::vector<float> dir = { 0.0f, 0.0f, 0.0f };
        float cutOff = 0.0f;

        switch (newLight->tipo) {
        case 'p': // point
            if (lightElem->Attribute("posx")) pos[0] = std::atof(lightElem->Attribute("posx"));
            if (lightElem->Attribute("posy")) pos[1] = std::atof(lightElem->Attribute("posy"));
            if (lightElem->Attribute("posz")) pos[2] = std::atof(lightElem->Attribute("posz"));
            break;

        case 'd': // directional
            if (lightElem->Attribute("dirx")) dir[0] = std::atof(lightElem->Attribute("dirx"));
            if (lightElem->Attribute("diry")) dir[1] = std::atof(lightElem->Attribute("diry"));
            if (lightElem->Attribute("dirz")) dir[2] = std::atof(lightElem->Attribute("dirz"));
            break;

        case 's': // spotlight
            if (lightElem->Attribute("posx")) pos[0] = std::atof(lightElem->Attribute("posx"));
            if (lightElem->Attribute("posy")) pos[1] = std::atof(lightElem->Attribute("posy"));
            if (lightElem->Attribute("posz")) pos[2] = std::atof(lightElem->Attribute("posz"));
            if (lightElem->Attribute("dirx")) dir[0] = std::atof(lightElem->Attribute("dirx"));
            if (lightElem->Attribute("diry")) dir[1] = std::atof(lightElem->Attribute("diry"));
            if (lightElem->Attribute("dirz")) dir[2] = std::atof(lightElem->Attribute("dirz"));
            if (lightElem->Attribute("cutoff")) cutOff = std::atof(lightElem->Attribute("cutoff"));
            break;
        }

        // Atribui os valores à nova luz.
        newLight->pos = new std::vector<float>(pos);
        newLight->dir = new std::vector<float>(dir);
        newLight->cutOff = cutOff;

        // Adiciona a nova luz ao vetor de luzes no mundo.
        lightsVec->push_back(newLight);
    }

    return lightsVec;
}



World parseXmlFile(World* world, const char* filePath) {
    TiXmlDocument file;
    if (file.LoadFile(filePath)) {
        TiXmlElement* root = file.FirstChildElement("world");

        // Parse window attributes
        TiXmlElement* windowElement = root->FirstChildElement("window");
        int width = atoi(windowElement->Attribute("width"));
        int height = atoi(windowElement->Attribute("height"));
        setWindow(world, width, height);

        // Parse camera attributes
        TiXmlElement* cameraElement = root->FirstChildElement("camera");
        TiXmlElement* position = cameraElement->FirstChildElement("position");
        float pos_x = atof(position->Attribute("x"));
        float pos_y = atof(position->Attribute("y"));
        float pos_z = atof(position->Attribute("z"));
        setCameraPosition(getCamera(*world), pos_x, pos_y, pos_z);

        TiXmlElement* lookAt = cameraElement->FirstChildElement("lookAt");
        float lookAt_x = atof(lookAt->Attribute("x"));
        float lookAt_y = atof(lookAt->Attribute("y"));
        float lookAt_z = atof(lookAt->Attribute("z"));
        setCameraLookAt(getCamera(*world), lookAt_x, lookAt_y, lookAt_z);

        TiXmlElement* up = cameraElement->FirstChildElement("up");
        float up_x = atof(up->Attribute("x"));
        float up_y = atof(up->Attribute("y"));
        float up_z = atof(up->Attribute("z"));
        setCameraUp(getCamera(*world), up_x, up_y, up_z);

        TiXmlElement* projection = cameraElement->FirstChildElement("projection");
        float fov = atof(projection->Attribute("fov"));
        float near = atof(projection->Attribute("near"));
        float far = atof(projection->Attribute("far"));
        setProjection(getProjection(getCamera(*world)), fov, near, far);

        setWorldGroups(*world, parseGroups(root->FirstChildElement("group")));
        setWorldLights(*world, parseLights(root->FirstChildElement("lights")));
        
    }
    else {
        std::cerr << "Failed to load XML file: " << filePath << std::endl;
    }

    return *world;
}

void printGroupRecursive(Arvore arvore) {
    if (arvore) {
        Group grupo = (Group)getDataArvore(arvore);
        if (grupo) {
            std::cout << "Transformações do grupo:" << std::endl;
            printTransforms(getGroupTransforms(grupo));

            std::cout << "Figuras do grupo:" << std::endl;
            printFiguras(getGroupFigures(grupo));

            LinkedList filhos = getFilhosArvore(arvore);
            LinkedList current = filhos;
            while (current != NULL) {
                printGroupRecursive((Arvore)getData(current));
                current = (LinkedList)getNext(current);
            }
        }
    }
}

void printWorld(World world) {
    if (world == nullptr) {
        std::cerr << "World não inicializado." << std::endl;
        return;
    }

    // Imprime informações sobre a janela
    std::cout << "Window:" << std::endl;
    std::cout << "Width: " << getWindowWidth(getWorldWindow(world)) << std::endl;
    std::cout << "Height: " << getWindowHeight(getWorldWindow(world)) << std::endl;

    // Imprime informações sobre a câmera
    std::cout << "Camera:" << std::endl;
    std::cout << "Position: (" << getCameraPosition(getWorldCamera(world))[0] << ", "
        << getCameraPosition(getWorldCamera(world))[1] << ", "
        << getCameraPosition(getWorldCamera(world))[2] << ")" << std::endl;
    std::cout << "LookAt: (" << getCameraLookAt(getWorldCamera(world))[0] << ", "
        << getCameraLookAt(getWorldCamera(world))[1] << ", "
        << getCameraLookAt(getWorldCamera(world))[2] << ")" << std::endl;
    std::cout << "Up: (" << getCameraUp(getWorldCamera(world))[0] << ", "
        << getCameraUp(getWorldCamera(world))[1] << ", "
        << getCameraUp(getWorldCamera(world))[2] << ")" << std::endl;

    // Imprime informações sobre a projeção da câmera
    Projection proj = getProjection(getWorldCamera(world));
    std::cout << "Projection:" << std::endl;
    std::cout << "FOV: " << getProjectionFOV(proj) << std::endl;
    std::cout << "Near: " << getProjectionNear(proj) << std::endl;
    std::cout << "Far: " << getProjectionFar(proj) << std::endl;


    // Imprime informações sobre todos os grupos
    Arvore grupos = getWorldGroups(world);
    if (grupos) {
        std::cout << "Grupos:" << std::endl;
        printGroupRecursive(grupos);
    }

    if (world->lights && !world->lights->empty()) {
        std::cout << "Lights:" << std::endl;
        for (const Light& light : *(world->lights)) {
            std::cout << "Type: " << light->tipo << std::endl;
            if (light->tipo == 'p' || light->tipo == 's') {
                std::cout << "Position: (" << (*light->pos)[0] << ", "
                    << (*light->pos)[1] << ", "
                    << (*light->pos)[2] << ")" << std::endl;
            }
            if (light->tipo == 'd' || light->tipo == 's') {
                std::cout << "Direction: (" << (*light->dir)[0] << ", "
                    << (*light->dir)[1] << ", "
                    << (*light->dir)[2] << ")" << std::endl;
            }
            if (light->tipo == 's') {
                std::cout << "CutOff: " << light->cutOff << std::endl;
            }
        }
    }
}
