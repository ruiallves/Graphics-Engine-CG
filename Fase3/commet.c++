#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>

void pontoSph(float alpha, float beta, float radius, float &x, float &y, float &z) {
    x = radius * cos(beta) * sin(alpha);
    y = radius * sin(beta);
    z = radius * cos(beta) * cos(alpha);
}

void rodaPonto(float x, float y, float z, float alpha, float &x_out, float &y_out, float &z_out) {
    float Rot[3][3] = {
        {cos(alpha), 0, sin(alpha)},
        {0, 1, 0},
        {-sin(alpha), 0, cos(alpha)}
    };
    x_out = Rot[0][0] * x + Rot[0][1] * y + Rot[0][2] * z;
    y_out = Rot[1][0] * x + Rot[1][1] * y + Rot[1][2] * z;
    z_out = Rot[2][0] * x + Rot[2][1] * y + Rot[2][2] * z;
}

std::vector<std::vector<float>> cometPatch(float alpha, float beta, float a_delta, float b_delta, float radius) {
    std::vector<std::vector<float>> patch;
    float x, y, z;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            pontoSph(alpha + a_delta * i, beta + b_delta * j, radius, x, y, z);
            std::vector<float> point = {x, y, z};
            patch.push_back(point);
        }
    }
    return patch;
}

std::vector<std::vector<std::vector<float>>> cometPatches(int slices, int stacks, float radius) {
    float a_delta = 2 * M_PI / slices;
    float b_delta = M_PI / (2 * stacks);
    std::vector<std::vector<std::vector<float>>> patches;
    for (int sl = 0; sl < slices; ++sl) {
        for (int st = 0; st < stacks; ++st) {
            std::vector<std::vector<float>> patchUp = cometPatch(a_delta * sl, b_delta * st, a_delta / 3, b_delta / 3, radius);
            std::vector<std::vector<float>> patchDown;
            for (auto &point : patchUp) {
                float x, y, z;
                rodaPonto(point[0], -point[1], point[2], a_delta / 2, x, y, z);
                std::vector<float> rotated_point = {x, y, z};
                patchDown.push_back(rotated_point);
            }
            patches.push_back(patchUp);
            patches.push_back(patchDown);
        }
    }
    return patches;
}

void patchesToFile(const std::string &filePath, const std::vector<std::vector<std::vector<float>>> &patches) {
    std::ofstream file(filePath);
    int numPatches = patches.size();
    int numControlPoints = numPatches * 16;
    file << numPatches << std::endl;
    for (int i = 0; i < numPatches; ++i) {
        for (int j = 0; j < 16; ++j) {
            file << i * 16 + j;
            if (j < 15) {
                file << ",";
            } else {
                file << std::endl;
            }
        }
    }
    file << numControlPoints << std::endl;
    for (const auto &patch : patches) {
        for (const auto &point : patch) {
            file << point[0] << "," << point[1] << "," << point[2] << std::endl;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 7) {
        std::cout << "Insira os seguintes argumentos por ordem:" << std::endl;
        std::cout << " 1) Seed para os números aleatórios" << std::endl;
        std::cout << " 2) \"Raio\" do cometa" << std::endl;
        std::cout << " 3) Altura máxima das irregularidades do cometa" << std::endl;
        std::cout << " 4) Slices do cometa (em termos de patches)" << std::endl;
        std::cout << " 5) Stacks do cometa (em termos de patches)" << std::endl;
        std::cout << " 6) Ficheiro onde serão guardados os patches" << std::endl;
        return 1;
    }

    int seed = std::atoi(argv[1]);
    srand(seed);

    float radius = std::atof(argv[2]);
    float max_height = std::atof(argv[3]);
    int slices = std::atoi(argv[4]);
    int stacks = std::atoi(argv[5]);
    std::string file = argv[6];

    auto patches = cometPatches(slices, stacks, radius);
    patchesToFile(file, patches);

    std::cout << "Done!" << std::endl;
    return 0;
}
