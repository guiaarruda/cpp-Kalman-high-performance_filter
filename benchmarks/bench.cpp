#include <chrono>
#include <cstdio>
#include <random>
#include "../src/core/Matrix.hpp"
#include "../src/physics/KalmanFilter.hpp"

using Clock = std::chrono::steady_clock;
static volatile double sink;   

Matrix randomMatrix(size_t n, std::mt19937& g) {
    std::uniform_real_distribution<double> d(-1.0, 1.0);
    Matrix M(n, n);
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < n; ++j) M(i,j) = d(g);
    return M;
}

template <class F>
double timeUs(F f, int reps) {
    auto t0 = Clock::now();
    for (int i = 0; i < reps; ++i) f();
    auto t1 = Clock::now();
    return std::chrono::duration<double, std::micro>(t1 - t0).count() / reps;
}

int main() {
    std::mt19937 g(42);
    std::printf("%-6s %14s %14s %9s\n", "N", "naive (us)", "optimized (us)", "speedup");
    for (size_t n : {4, 16, 64, 128, 256, 512}) {
        Matrix A = randomMatrix(n, g), B = randomMatrix(n, g);
        int reps = n <= 16 ? 200000 : n <= 128 ? 200 : 5;
        double tn = timeUs([&]{ sink = A.multiplyNaive(B)(0,0); }, reps);
        double to = timeUs([&]{ sink = (A * B)(0,0); }, reps);
        std::printf("%-6zu %14.3f %14.3f %8.1fx\n", n, tn, to, tn / to);
    }

    const double dt = 0.1;
    KalmanFilter kf(4, 2);
    Matrix F = Matrix::indentity(4); F(0,2) = dt; F(1,3) = dt; kf.setF(F);
    Matrix H(2, 4); H(0,0) = 1; H(1,1) = 1; kf.setH(H);
    Matrix Q = Matrix::indentity(4); for (int i = 0; i < 4; ++i) Q(i,i) = 0.1; kf.setQ(Q);
    Matrix R(2, 2); R(0,0) = 2; R(1,1) = 2; kf.setR(R);
    Matrix P0 = Matrix::indentity(4); for (int i = 0; i < 4; ++i) P0(i,i) = 1000;
    kf.setInitialState(Matrix(4, 1), P0);
    Matrix z(2, 1); z(0,0) = 1.0; z(1,0) = 0.5;

    const int STEPS = 1000000;
    double us = timeUs([&]{ kf.predict(); kf.update(z); }, STEPS);
    sink = kf.getState()(0,0);
    std::printf("\nKalman step (predict+update): %.3f us\n", us);
    return 0;
}