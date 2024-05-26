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

void getGlobalCatmullRomPoint(float gt, std::vector<std::vector<float>> controlPoints, float* pos, float* deriv) {
	size_t POINT_COUNT = controlPoints.size();
	float t = gt * POINT_COUNT; 
	int index = floor(t);
	t = t - index; 

	int indices[4];
	indices[0] = (index + POINT_COUNT - 1) % POINT_COUNT;
	indices[1] = (indices[0] + 1) % POINT_COUNT;
	indices[2] = (indices[1] + 1) % POINT_COUNT;
	indices[3] = (indices[2] + 1) % POINT_COUNT;

	getCatmullRomPoint(t, controlPoints[indices[0]], controlPoints[indices[1]], controlPoints[indices[2]], controlPoints[indices[3]], pos, deriv);
}

void normalize(float* a) {
    float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0] / l;
    a[1] = a[1] / l;
    a[2] = a[2] / l;
}

void cross(const float* a, const float* b, float* res) {
	res[0] = a[1] * b[2] - a[2] * b[1];
	res[1] = a[2] * b[0] - a[0] * b[2];
	res[2] = a[0] * b[1] - a[1] * b[0];
}

void buildRotMatrix(const float* x, const float* y, const float* z, float* m) {
    m[0] = x[0];
	m[1] = x[1];
	m[2] = x[2];
	m[3] = 0;
	m[4] = y[0];
	m[5] = y[1];
	m[6] = y[2];
	m[7] = 0;
	m[8] = z[0];
	m[9] = z[1];
	m[10] = z[2];
	m[11] = 0;
	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	m[15] = 1;
}