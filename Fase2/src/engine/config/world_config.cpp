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
    LinkedList grupos;
};

struct group {
    LinkedList transformacoes;
    LinkedList figuras;
};

struct transform {
    char tipo;
    float x, y, z;
    float anguloR;
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

Transform newTransform(char tipo, float x, float y, float z, float angle) {
    Transform transform = (Transform)malloc(sizeof(struct transform));
    if (transform) {
        transform->tipo = tipo;
        transform->x = x;
        transform->y = y;
        transform->z = z;
        transform->anguloR = angle;
    }
    return transform;
}

World newConfig() {
    World newConf = (World)malloc(sizeof(struct world));
    if (newConf) {
        newConf->camera = newCamera();
        newConf->camera->projection = newProjection();
        newConf->grupos = newLinkedListEmpty();
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

LinkedList getWorldGroups(World world) {
    return world->grupos;
}

void setWorldGroups(World world, LinkedList grupos) {
    world->grupos = grupos;
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

void addTransform(LinkedList ll, Transform transform) {
    if (ll && transform) {
        Group group = (Group)getData(ll);
        LinkedList transforms = getGroupTransforms(group);
        LinkedList newTransformNode = newLinkedList(transform);
        addChild(&transforms, transform);
        setGroupTransforms(group, transforms);
    }
}


void addModel(LinkedList ll, const char* model) {
    if (ll && model) {
        Figura fig = fileToFigura(model);
        Group group = (Group)getData(ll);
        LinkedList figuras = getGroupFigures(group);
        addChild(&figuras, fig);
    }
}

LinkedList parseGroups(TiXmlElement* root) {
    if (root) {
        LinkedList res = newLinkedList(newGroup());

        // METER AS TRANSFORMS NA ARVORE
        Transform transform_obj = NULL;
        TiXmlElement* transform = root->FirstChildElement("transform");
        if (transform) {
            for (TiXmlElement* t = transform->FirstChildElement(); t; t = t->NextSiblingElement()) {
                const char* name_of_transform = t->Value();
                float angle = 0.0f;
                if (strcmp(name_of_transform, "rotate") == 0) {
                    angle = atof(t->Attribute("angle"));
                }
                float x = atof(t->Attribute("x"));
                float y = atof(t->Attribute("y"));
                float z = atof(t->Attribute("z"));
                transform_obj = newTransform(name_of_transform[0], x, y, z, angle);
                addTransform(res, transform_obj);
            }
        }

        //Group groupt = (Group)getData(res);
        //LinkedList trans = getGroupTransforms(groupt);
        //Transform t = (Transform)getData(trans);
        //cout << t->x;

        // METER OS MODELS NA ARVORE
        TiXmlElement* group = root->FirstChildElement("group");
        TiXmlElement* models = group->FirstChildElement("models");
        if (models) {
            for (TiXmlElement* m = models->FirstChildElement("model"); m; m = m->NextSiblingElement("model")) {
                const char* file_name = m->Attribute("file");
                addModel(res, file_name);
            }
        }

        //Group groupt = (Group)getData(res);
        //LinkedList figs = getGroupFigures(groupt);
        //Figura f = (Figura)getData(figs);

        //vector<Vertice> vertices = getFiguraVertices(f);
        //cout << vertices[0]->x;

        for (TiXmlElement* chGroup = group->FirstChildElement("group"); chGroup; chGroup = chGroup->NextSiblingElement("group")) {
            LinkedList child = parseGroups(chGroup);
            addChild(&res, child);
        }
        return res;
    }
    return NULL;
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

        setWorldGroups(*world, parseGroups(root));
    }
    else {
        std::cerr << "Failed to load XML file: " << filePath << std::endl;
    }

    return *world;
}

void printWorld(World world) {
    std::cout << "Window width: " << getWindowWidth(getWorldWindow(world)) << std::endl;
    std::cout << "Window height: " << getWindowHeight(getWorldWindow(world)) << std::endl;

    Camera camera = getWorldCamera(world);
    std::cout << "Camera position: (" << getCameraPosition(camera)[0] << ", "
        << getCameraPosition(camera)[1] << ", "
        << getCameraPosition(camera)[2] << ")" << std::endl;
    std::cout << "Camera lookAt: (" << getCameraLookAt(camera)[0] << ", "
        << getCameraLookAt(camera)[1] << ", "
        << getCameraLookAt(camera)[2] << ")" << std::endl;
    std::cout << "Camera up: (" << getCameraUp(camera)[0] << ", "
        << getCameraUp(camera)[1] << ", "
        << getCameraUp(camera)[2] << ")" << std::endl;

    Projection projection = getProjection(camera);
    std::cout << "Projection FOV: " << getProjectionFOV(projection) << std::endl;
    std::cout << "Projection near: " << getProjectionNear(projection) << std::endl;
    std::cout << "Projection far: " << getProjectionFar(projection) << std::endl;
}