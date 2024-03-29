#include "../../tools/figura.h"
#include <cmath> 

Figura createSphere(float radius, float slices, float stacks);
Figura createPlane(int length, int divisions);
Figura createCone(float radius, float height, int slices, int stacks);
Figura createBox(float size, int divisions);
Figura createRing(float ri, float re, int slices);