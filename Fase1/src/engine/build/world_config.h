#include "../../tinyxml/tinyxml.h"
#include <iostream>
#include <string>
#include <vector>


struct Window {
    int width;
    int height;
};

struct Projection {
    float fov;
    float near;
    float far;
};

struct Camera {
    float position[3];
    float lookAt[3];
    float up[3];
    Projection projection;
};

struct world {
    Window window;
    Camera camera;
    std::string* files;
    int numFiles; 
};

typedef struct world* World;

World parseXmlFile(World* world, const char* filePath);

World newConfig();