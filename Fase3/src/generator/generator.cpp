#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include "CreateVertice/createvertice.h"
#include "../tinyxml/tinyxml.h"

#define PLANE 0
#define SPHERE 1
#define BOX 2
#define CONE 3
#define RING 4
#define SURFACE 5
TiXmlDocument doc;


#include <vector>  // Para armazenamento eficiente de vértices
#include <string>

using namespace std;


int createFileType(LinkedList vertices, string name, int arg1, int arg2, int arg3, int arg4) {
    // Encontrar a posição do ponto
    auto dot_pos = name.find('.');

    // Obter a substring antes do ponto
    std::string prefix = name.substr(0, dot_pos);

    // Obter a substring após o ponto
    std::string suffix = name.substr(dot_pos);

    string filePath;

    if (arg2 == -1) {
		filePath = "../../../outputs/" + prefix + "_" + std::to_string(arg1) + suffix;
	}
	else if (arg3 == -1) {
        filePath = "../../../outputs/" + prefix + "_" + std::to_string(arg1) + "_" + std::to_string(arg2) + suffix;
    }
    else if (arg4 == -1) {
        filePath = "../../../outputs/" + prefix + "_" + std::to_string(arg1) + "_" + std::to_string(arg2) + "_" + std::to_string(arg3) + suffix;
    }
    else {
        filePath = "../../../outputs/" + prefix + "_" + std::to_string(arg1) + "_" + std::to_string(arg2) + "_" + std::to_string(arg3) + "_" + std::to_string(arg4) + suffix; // Nome do arquivo com extensão .3d
    }

    ofstream file(filePath);

    if (!file.is_open()) {
        cout << "Erro ao criar o arquivo " << filePath << endl;
        return -1;
    }

    LinkedList currentVertice = vertices;
    while (currentVertice != nullptr) {
        Vertice v = (Vertice)getData(currentVertice);
        if (v != nullptr) {
            file << getVerticeX(v) << "," << getVerticeY(v) << "," << getVerticeZ(v) << endl;
        }
        currentVertice = (LinkedList)getNext(currentVertice);
    }

    file.close();
    cout << "Arquivo " << filePath << " criado com sucesso." << endl;

    return 0;
}


int main(int argc, char** argv) {

    //Phase 1 – Graphical primitives
    /*
        Generator
        To create the model files an application(independent from the engine) will receive as parameters
        the graphical primitive’s type, other parameters required for the model creation, and the
        destination file where the vertices will be stored.
        In this phase the following graphical primitives are required :

            Plane(a square in the XZ plane, centered in the origin, subdivided in both X and Z directions)
            Box(requires dimension, and the number of divisions per edge, centered in the origin)
            Sphere(requires radius, slices and stacks, , centered in the origin)
            Cone(requires bottom radius, height, slices and stacks, the bottom of the cone should be
            on the XZ plane)

        Both the box and sphere should be centered on the origin.The cone should have its base in the XZ
        plane.
        For instance, if we wanted to create a sphere with radius 1, 10 slices, 10 stacks, and store the
        resulting vertices in a file named sphere.3d, and assuming our application is called generator, we
        could write on a terminal :

            C:\ > generator sphere 1 10 10 sphere.3d
            Instruction to generate a box with a length of 2 units, where each side is divided in a grid 3x3:
            C:\ > generator box 2 3 box.3d
            To generate a cone with radius 1, height 2, 4 slices and 3 stacks, we could write :
            C:\ > generator cone 1 2 4 3 cone.3d
            Finally, here is an example to create a plane with 1 unit in length, and 3 divisions along each axis :
            C:\ > generator plane 1 3 plane.3d

        After the above commands are complete there should be a set of files with “3d” extension, each
        file containing all the vertices and faces required to define the respective primitive.
        The file format should be defined by the students and can support additional information to assist
        the reading process, for example, the file may contain a line at the beginning stating the number
        of vertices it contains.*/

        //generate a file with plane in witch the arguments are "generator plane 1 3 plane.3d"
        //argv[0] -> name of program
        //argv[1] -> generate a plane
        //argv[2] -> 1 unit lengh
        //argv[3] -> 3 divisions for each axis
        //argv[4] -> name of the output file to be saved in "../../outputs" with ".3d" extension (não assumimos )

        //ERROR CODES:
        /*
            1 - Numero de argumentos inválidos para o tipo de objeto referenciado no argv[1].
            2 - Invalid input (tipo desconhecido).
            3 -
        */

    if (argc < 4 || argc > 7) return 1; //Argument Eval

    int  shapeType = -1;
    Figura figura;

    if (strcmp(argv[1], "sphere") == 0) shapeType = SPHERE;
    else if (strcmp(argv[1], "box") == 0) shapeType = BOX;
    else if (strcmp(argv[1], "cone") == 0) shapeType = CONE;
    else if (strcmp(argv[1], "plane") == 0) shapeType = PLANE;
    else if (strcmp(argv[1], "ring") == 0) shapeType = RING;
    else if (strcmp(argv[1], "surface") == 0) shapeType = SURFACE;

    //printf("%s\n", argv[0]);
    //printf("%s\n", argv[1]);
    //printf("%s\n", argv[2]);
    //printf("%s\n", argv[3]);
    //printf("%d\n", shapeType);

    switch (shapeType)
    {

    case PLANE:

        //argument control
        if (argc != 5) {
            printf("Wrong Number of arguments!");
            return 1;
        }

        //printf("got PLANE ==DEBUG TO REMOVE==");

        //build PLANE
        figura = createPlane(std::stof(argv[2]), std::stof(argv[3]));
        createFileType(getFiguraVertices(figura), argv[4], std::stof(argv[2]), std::stof(argv[3]), -1, -1);

        break;

    case SPHERE:

        //argument control
        if (argc != 6) {

            printf("Wrong Number of arguments!");
            return 1;

        }

        //printf("got SPHERE ==DEBUG TO REMOVE==");

        //build SPHERE
        figura = createSphere(std::stof(argv[2]), std::stof(argv[3]), std::stof(argv[4]));
        createFileType(getFiguraVertices(figura), argv[5], std::stof(argv[2]), std::stof(argv[3]), std::stof(argv[4]), -1);
        break;

    case BOX:

        //argument control
        if (argc != 5) {

            printf("Wrong Number of arguments!");
            return 1;

        }

        //printf("got BOX ==DEBUG TO REMOVE==");

        //build BOX
        figura = createBox(std::stof(argv[2]), std::stof(argv[3]));
        createFileType(getFiguraVertices(figura), argv[4], std::stof(argv[2]), std::stof(argv[3]), -1, -1);

        break;

    case CONE:

        //argument control
        if (argc != 7) {

            printf("Wrong Number of arguments!");
            return 1;

        }

        //printf("got CONE ==DEBUG TO REMOVE==");

        //build CONE
        figura = createCone(std::stof(argv[2]), std::stof(argv[3]), std::stof(argv[4]), std::stof(argv[5]));
        createFileType(getFiguraVertices(figura), argv[6], std::stof(argv[2]), std::stof(argv[3]), std::stof(argv[4]), std::stof(argv[5]));
        break;

    case RING:

        //argument control
        if (argc != 6) {

            printf("Wrong Number of arguments!");
            return 1;

        }

        // printf("got RING ==DEBUG TO REMOVE==");

         //build RING
        figura = createRing(std::stof(argv[2]), std::stof(argv[3]), std::stof(argv[4]));
        createFileType(getFiguraVertices(figura), argv[5], std::stof(argv[2]), std::stof(argv[3]), std::stof(argv[4]), -1);
        break;

    case SURFACE:

        //argument control
        if (argc != 5) {

            printf("Wrong Number of arguments!");
            return 1;

        }

        //printf("got SURFACE ==DEBUG TO REMOVE==");

        //build SURFACE
        figura = createBezier(argv[2], std::stof(argv[3]));
        //printVerticesLinkedList(getFiguraVertices(figura));
        createFileType(getFiguraVertices(figura), argv[4], std::stof(argv[3]), -1, -1, -1);
        break;

    default:

        printf("FAILED to get valid input (tipo desconhecido) ==DEBUG TO REMOVE==");
        return 2;
        break;
    }

    return 0;
}