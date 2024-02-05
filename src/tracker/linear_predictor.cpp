#include <iostream>
#include <emscripten/bind.h>

#include "linear_predictor.h"
#include "utils.cpp"

using namespace emscripten;
using namespace std;

// only used for coding, comment it when building
// #include <eigen3/Eigen/Core>

bool LinearPredictor::load(int numberOfPoints, int levels,
    float* i0, float* u0, int* m, float** As)
{
    this->n = numberOfPoints;
    this->levels = levels;
    this->i0 = Eigen::Map<Eigen::VectorXf>(i0, numberOfPoints);
    this->i1 = Eigen::VectorXf::Zero(numberOfPoints);

    this->u0 = Eigen::Map<Eigen::VectorXf>(u0, 8);

    this->m = m; // Eigen::Map<Eigen::VectorXi>(m, numberOfPoints);
    this->As = new Eigen::MatrixXf[levels];
    for (int i = 0; i < levels; i++) {
        this->As[i] = Eigen::Map<Eigen::MatrixXf>(As[i], 8, numberOfPoints);
    }

    this->H = Eigen::Matrix3f::Identity();

    return true;
}

void LinearPredictor::set_corners(float u0, float v0,
    float u1, float v1,
    float u2, float v2,
    float u3, float v3)
{
    this->u = Eigen::Map<Eigen::VectorXf>(new float[8]{u0, v0, u1, v1, u2, v2, u3, v3}, 8);
    homography_from_4corresp(this->H, this->u0, this->u);
}

bool LinearPredictor::track(float* imageDataArray, int width, int height) {
    for(int level=0; level < this->levels; level++) {
        for (int iter = 0; iter < 5; iter++) {
            for (int i = 0; i < this->n; i++) {
                int x1, y1;
                homography_transform(this->H, float(this->m[2*i]), float(this->m[2*i+1]), x1, y1);
                if (x1 < 0 || y1 < 0 || x1 >= width || y1 >= height)
	                return false;

                this->i1[i] = imageDataArray[y1 * width + x1];
            }

            if (!normalize(this->i1)) return false;
            this->di = this->i1 - this->i0;
            this->du = this->As[level] * this->di;
            this->u = this->u0 - this->du;
            homography_from_4corresp(this->H, this->u0, this->u);
        }
    }

    return true;
}

// embind
EMSCRIPTEN_BINDINGS (c) {
  class_<LinearPredictor>("LinearPredictor")
    .constructor()
    .function("load", &LinearPredictor::load, allow_raw_pointers())
    .function("set_corners", &LinearPredictor::set_corners)
    .function("track", &LinearPredictor::track, allow_raw_pointers());
}
