#include "world_config.h"

World newConfig() {
    World newConf = (World)malloc(sizeof(struct world));
    return newConf;
}

World parseXmlFile(World* world, const char* filePath) {
    TiXmlDocument file;
    if (file.LoadFile(filePath)) {
        TiXmlElement* root = file.FirstChildElement("world");

        // Parse window attributes
        TiXmlElement* window = root->FirstChildElement("window");
        (*world)->window.width = atoi(window->Attribute("width"));
        (*world)->window.height = atoi(window->Attribute("height"));

        // Parse camera attributes
        TiXmlElement* camera = root->FirstChildElement("camera");
        TiXmlElement* position = camera->FirstChildElement("position");
        (*world)->camera.position[0] = atof(position->Attribute("x"));
        (*world)->camera.position[1] = atof(position->Attribute("y"));
        (*world)->camera.position[2] = atof(position->Attribute("z"));

        TiXmlElement* lookAt = camera->FirstChildElement("lookAt");
        (*world)->camera.lookAt[0] = atof(lookAt->Attribute("x"));
        (*world)->camera.lookAt[1] = atof(lookAt->Attribute("y"));
        (*world)->camera.lookAt[2] = atof(lookAt->Attribute("z"));

        TiXmlElement* up = camera->FirstChildElement("up");
        (*world)->camera.up[0] = atof(up->Attribute("x"));
        (*world)->camera.up[1] = atof(up->Attribute("y"));
        (*world)->camera.up[2] = atof(up->Attribute("z"));

        TiXmlElement* projection = camera->FirstChildElement("projection");
        (*world)->camera.projection.fov = atof(projection->Attribute("fov"));
        (*world)->camera.projection.near = atof(projection->Attribute("near"));
        (*world)->camera.projection.far = atof(projection->Attribute("far"));

        TiXmlElement* group = root->FirstChildElement("group");
        TiXmlElement* models = group->FirstChildElement("models");
        if (models) {
            (*world)->numFiles = 0;
            (*world)->files = nullptr;
            for (TiXmlElement* model = models->FirstChildElement("model"); model; model = model->NextSiblingElement("model")) {
                const char* filename = model->Attribute("file");
                if (filename) {
                    std::string* newFiles = new std::string[(*world)->numFiles + 1];
                    for (int i = 0; i < (*world)->numFiles; ++i) {
                        newFiles[i] = (*world)->files[i];
                    }
                    newFiles[(*world)->numFiles++] = filename;
                    delete[](*world)->files;
                    (*world)->files = newFiles;
                }
            }
        }

    }

    else {
        std::cerr << "Failed to load XML file: " << filePath << std::endl;
    }

    return *world;
}