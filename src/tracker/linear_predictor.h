#ifndef linear_predictor
#define linear_predictor
#include <cmath>
#include <Eigen/Core>


class LinearPredictor {

    public:
        LinearPredictor(){};

        bool load(int numberOfPoints, int levels, float* i0, float* u0, int* m, float** As);

        void set_corners(float u0, float v0,
            float u1, float v1,
            float u2, float v2,
            float u3, float v3);

        bool track(float* imageDataArray, int width, int height);

    // private:
        int n;
        int levels;
        Eigen::MatrixXf* As;
        // Eigen::VectorXi m;
        int* m;
        Eigen::VectorXf i0;
        Eigen::VectorXf i1;
        Eigen::VectorXf di;
        Eigen::VectorXf u0;
        Eigen::VectorXf u;
        Eigen::VectorXf du;
        Eigen::Matrix3f H;
};

#endif