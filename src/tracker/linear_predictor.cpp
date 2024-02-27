#include <iostream>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include "linear_predictor.h"
#include "utils.cpp"

using namespace emscripten;
using namespace std;

// only used for coding, comment it when building
// #include <eigen3/Eigen/Core>

bool LinearPredictor::load(int numberOfPoints, int levels,
    float* i0, float* u0, int* m, float* As)
{
    this->n = numberOfPoints;
    this->levels = levels;
    this->i0 = Eigen::Map<Eigen::VectorXf>(i0, numberOfPoints);
    this->i1 = Eigen::VectorXf::Zero(numberOfPoints);

    this->u0 = Eigen::Map<Eigen::VectorXf>(u0, 8);

    this->m = Eigen::Map<Eigen::VectorXi>(m, numberOfPoints * 2);
    this->As = new Eigen::MatrixXf[levels];
    for (int i = 0; i < levels; i++) {
        this->As[i] = Eigen::Map<Eigen::Matrix<float, -1, -1, Eigen::RowMajor>>(As + i * numberOfPoints * 8, 8, numberOfPoints);
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

val LinearPredictor::get_corners() {
    size_t bufferLength1 = 8; // sizeof(this->uArray)/sizeof(float);
    float *byteBuffer1 = this->uArray;

    for (int i = 0; i < 4; i++) {
        homography_transform(this->H, float(this->u0[2*i]), float(this->u0[2*i+1]), this->uArray[2*i], this->uArray[2*i+1]);
    }

    val result = val(typed_memory_view(bufferLength1, byteBuffer1));
    return result;
}

bool LinearPredictor::track(int* imageDataArray, int width, int height) {
    for(int level=0; level < this->levels; level++) {
        for (int iter = 0; iter < 5; iter++) {
            for (int i = 0; i < this->n; i++) {
                int x1, y1;
                homography_transform(this->H, float(this->m[2*i]), float(this->m[2*i+1]), x1, y1);
                // if (level ==0 && iter == 1) {
                //     cout << "(" << x1 << ", " << y1 << ") ";
                // }
                if (x1 < 0 || y1 < 0 || x1 >= width || y1 >= height) {
                    cout << "track failed when do homography transform" << endl;
                    cout << "x1: " << x1 << " y1: " << y1 << " m: " << float(this->m[2*i]) << ", " << float(this->m[2*i+1]) << ", " << width << ", " << height << endl;
                    cout << this->H << endl;
	                return false;
                }

                int index = 4 * (y1 * width + x1);
                this->i1[i] = float(imageDataArray[index]) * 0.299 + float(imageDataArray[index+1]) * 0.587 + float(imageDataArray[index+2]) * 0.114;
            }
            if (!normalize(this->i1)) {
                cout << "track failed when normalization i1" << endl;
                return false;
            }

            this->di = this->i1 - this->i0;
            this->du = this->As[level] * this->di;
            Eigen::VectorXf u2 = this->u0 - this->du;

            Eigen::Matrix3f fs_homo;
            homography_from_4corresp(fs_homo, this->u0, u2);

            this->H = this->H * fs_homo;
            this->H.normalize();
        }
    }

    return true;
}

// embind
EMSCRIPTEN_BINDINGS (c) {
    class_<LinearPredictor>("LinearPredictor")
        .constructor()
        .function("set_corners", &LinearPredictor::set_corners)
        .function("track", optional_override([](LinearPredictor& lp, int imageDataArray, int width, int height) {
            // malloc in js and pass ptr to wasm
            return lp.track((int*)imageDataArray, width, height);
        }))
        .function("load", optional_override([](LinearPredictor& lp, int numberOfPoints, int levels, val i0, val u0, val m, val As) {
            std::vector<float> i0Vector = convertJSArrayToNumberVector<float>(i0);
            std::vector<float> u0Vector = convertJSArrayToNumberVector<float>(u0);
            std::vector<int> mVector = convertJSArrayToNumberVector<int>(m);
            std::vector<float> AsVector = convertJSArrayToNumberVector<float>(As);
            return lp.load(numberOfPoints, levels, i0Vector.data(), u0Vector.data(), mVector.data(), AsVector.data());
        }))
        .function("get_corners", &LinearPredictor::get_corners);
}
