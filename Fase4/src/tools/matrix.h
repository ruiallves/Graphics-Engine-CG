#include <vector>

void getGlobalCatmullRomPoint(float gt, std::vector<std::vector<float>> controlPoints, float* pos, float* deriv);

void normalize(float* a);
void cross(const float* a, const float* b, float* res);
void buildRotMatrix(const float* x, const float* y, const float* z, float* m);