#include "matrix.h"

void getCatmullRomPoint(float t, const std::vector<float>& p0, const std::vector<float>& p1,
    const std::vector<float>& p2, const std::vector<float>& p3,
    float* pos, float* deriv) {

    float t2 = t * t;
    float t3 = t2 * t;
    for (int i = 0; i < 3; ++i) {
        pos[i] = 0.5f * ((2 * p1[i]) +
            (-p0[i] + p2[i]) * t +
            (2 * p0[i] - 5 * p1[i] + 4 * p2[i] - p3[i]) * t2 +
            (-p0[i] + 3 * p1[i] - 3 * p2[i] + p3[i]) * t3);
        deriv[i] = 0.5f * ((-p0[i] + p2[i]) +
            (4 * p0[i] - 10 * p1[i] + 8 * p2[i] - 2 * p3[i]) * t +
            (-3 * p0[i] + 9 * p1[i] - 9 * p2[i] + 3 * p3[i]) * t2);
    }
}

void getGlobalCatmullRomPoint(float gt, const std::vector<std::vector<float>>& controlPoints,
    float* pos, float* deriv) {
    int numSections = controlPoints.size() - 3;
    int currentIndex = static_cast<int>(gt * numSections);
    float localt = (gt * numSections) - currentIndex;

    int p0Index = currentIndex == 0 ? currentIndex : currentIndex - 1;
    int p1Index = currentIndex;
    int p2Index = currentIndex + 1;
    int p3Index = currentIndex + 2;

    const std::vector<float>& p0 = controlPoints[p0Index];
    const std::vector<float>& p1 = controlPoints[p1Index];
    const std::vector<float>& p2 = controlPoints[p2Index];
    const std::vector<float>& p3 = controlPoints[p3Index];

    getCatmullRomPoint(localt, p0, p1, p2, p3, pos, deriv);
}
