#ifndef linear_predictor
#define linear_predictor
#include <cmath>
#include <Eigen/Core>
#include <emscripten/val.h>


class LinearPredictor {

    public:
        LinearPredictor(){
            this->uArray = new float[8]{1, 2, 3, 4, 5, 6, 7, 8};
        };

        bool load(int numberOfPoints, int levels, float* i0, float* u0, int* m, float* As);

        int processArray(int* inputArray, int length);

        void add(const float* a, const float* b, float* c, int len);

        void set_corners(float u0, float v0,
            float u1, float v1,
            float u2, float v2,
            float u3, float v3);

        emscripten::val get_corners();

        bool track(int* imageDataArray, int width, int height);

    // private:
        int n;
        int levels;
        Eigen::MatrixXf* As;
        Eigen::VectorXi m;
        Eigen::VectorXf i0;
        Eigen::VectorXf i1;
        Eigen::VectorXf di;
        Eigen::VectorXf u0;
        Eigen::VectorXf u;
        Eigen::VectorXf du;
        Eigen::Matrix3f H;

        float* uArray;
};

#endif