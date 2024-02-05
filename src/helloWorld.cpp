#include "helloWorld.h"
#include <emscripten/bind.h>
using namespace emscripten;

int example_eigen(){

    // Basic Example of cpp
    std::cout << "Example of cpp \n";
    float a = 1.0, b = 2.0;
    std::cout << a << std::endl;
    std::cout << a/b << std::endl;
    std::cout << std::sqrt(b) << std::endl;
    std::cout << std::acos(-1) << std::endl;
    std::cout << std::sin(30.0/180.0*acos(-1)) << std::endl;

    // Example of vector
    std::cout << "Example of vector \n";
    // vector definition
    Eigen::Vector3f v(1.0f,2.0f,3.0f);
    Eigen::Vector3f w(1.0f,0.0f,0.0f);
    // vector output
    std::cout << "Example of output \n";
    std::cout << v << std::endl;
    // vector add
    std::cout << "Example of add \n";
    std::cout << v + w << std::endl;
    // vector scalar multiply
    std::cout << "Example of scalar multiply \n";
    std::cout << v * 3.0f << std::endl;
    std::cout << 2.0f * v << std::endl;

    // Example of matrix
    std::cout << "Example of matrix \n";
    // matrix definition
    Eigen::Matrix3f i,j;
    i << 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0;
    j << 2.0, 3.0, 1.0, 4.0, 6.0, 5.0, 9.0, 7.0, 8.0;
    // matrix output
    std::cout << "Example of output \n";
    std::cout << i << std::endl;
    // matrix add i + j
    // matrix scalar multiply i * 2.0
    // matrix multiply i * j
    // matrix multiply vector i * v

    return 0;
}

int testEigen() {
    // given a point p= (2,1), give a rotation theta = 45 degree, and translation t = (1,2), compute the new point p'
    // homogeneous coordinates is required
    Eigen::Vector3f p(2.0f, 1.0f, 1.0f);
    float theta = 45.0f / 180.0f * acos(-1);
    Eigen::Matrix3f transform;
    transform << cos(theta), -sin(theta), 1, sin(theta), cos(theta), 2, 0, 0, 1;

    Eigen::Vector3f p_;
    p_ = transform * p;
    std::cout << "p1(2.0, 1.0, 1.0)" << std::endl;
    std::cout << "transform matrix is:" << std::endl;
    std::cout << cos(theta) << "\t" << -sin(theta) << "\t" << 1 << std::endl;
    std::cout << sin(theta) << "\t" << cos(theta) << "\t" << 2 << std::endl;
    std::cout << 0 << "\t" << 0 << "\t" << 1 << std::endl;
    std::cout << "p2(" <<  p_[0] << ", " << p_[1] << ')'<< std::endl;

    p_ = p_ / 2.0f;
    std::cout << p_ << std::endl;

    transform(2, 2) = 1.1;
    std::cout << transform << std::endl;

    Eigen::Vector2f a(2.5f, 3.5f);
    Eigen::Vector3f b(a[0], a[1], 1.0f);
    std::cout << b << std::endl;

    Eigen::Vector3f u;
    u << 233.0f, 123.0f, 1.0f;
    std::cout << u << std::endl;

    float c[8] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f};
    Eigen::Map<Eigen::VectorXf> i0(c, 8);

    i0 = i0 - (Eigen::VectorXf::Ones(i0.size()) * i0.mean());

    std::cout << i0 << std::endl << i0.mean() << std::endl;

    return 0;
}

int benchmarkEigen(int counts) {
    Eigen::MatrixXf m = Eigen::MatrixXf::Random(8,100);
    Eigen::MatrixXf m2 = Eigen::MatrixXf::Random(100,400);
    Eigen::VectorXf v = Eigen::VectorXf::Random(400);

    for (int i = 0; i < counts; ++i)
    {
        Eigen::VectorXf result = m * m2 * v;
    }

    return 0;
}

int main() {
    testEigen();
    example_eigen();
    benchmarkEigen(10);
}

// embind
EMSCRIPTEN_BINDINGS(core_module) {
    function("example_eigen", &example_eigen);
    function("testEigen", &testEigen);
    function("benchmarkEigen", &benchmarkEigen);
}