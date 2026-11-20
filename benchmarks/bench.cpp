// Benchmarks:
//  1. Matrix multiplication: the original version (i-j-k loop order with bounds-checked
//     access) versus the current Matrix::operator* (i-k-j order over contiguous rows).
//  2. Latency of one Kalman filter step (predict + update) for the 4-state tracker.
#include<iostream>
#include<iomanip>
#include<chrono>
#include<random>
#include<vector>
#include "../src/core/Matrix.hpp"
#include "../src/physics/KalmanFilter.hpp"
using namespace std;
using Clock = chrono::steady_clock;

// Reference implementation equal to the first version of Matrix::operator*.
static Matrix multiplyNaive(const Matrix& A, const Matrix& B){
    Matrix result(A.getRows(), B.getColumns(), 0.0);
    for(size_t i = 0; i < A.getRows(); i++)
        for(size_t j = 0; j < B.getColumns(); j++)
            for(size_t k = 0; k < A.getColumns(); k++)
                result(i, j) += A(i, k) * B(k, j);
    return result;
}

static Matrix randomMatrix(size_t n, mt19937& gen){
    uniform_real_distribution<> dist(-1.0, 1.0);
    Matrix M(n, n);
    for(size_t i = 0; i < n; i++)
        for(size_t j = 0; j < n; j++)
            M(i,j) = dist(gen);
    return M;
}

static volatile double sink;  // keeps the compiler from removing the measured work

template<class F>
static double bestTimeMs(F f, int reps){
    double best = 1e300;
    for(int r = 0; r < reps; r++){
        auto t0 = Clock::now();
        f();
        auto t1 = Clock::now();
        best = min(best, chrono::duration<double, milli>(t1 - t0).count());
    }
    return best;
}

int main(){
    mt19937 gen(7);
    cout << fixed << setprecision(3);
    cout << "== Matrix multiplication (best of 5 runs) ==\n";
    cout << "    N |  naive i-j-k (ms) | optimized i-k-j (ms) | speedup\n";
    for(size_t n : {64, 128, 256, 512}){
        Matrix A = randomMatrix(n, gen), B = randomMatrix(n, gen);
        double t_naive = bestTimeMs([&]{ sink = multiplyNaive(A, B)(0,0); }, 5);
        double t_opt   = bestTimeMs([&]{ sink = (A * B)(0,0); }, 5);
        cout << setw(5) << n << " | " << setw(17) << t_naive << " | "
             << setw(20) << t_opt << " | " << setprecision(1) << setw(6)
             << t_naive / t_opt << "x\n" << setprecision(3);
    }

    cout << "\n== Kalman filter step (4 states, 2 measurements) ==\n";
    const double dt = 0.1;
    KalmanFilter kf(4, 2);
    Matrix F = Matrix::identity(4); F(0,2) = dt; F(1,3) = dt;
    Matrix H(2, 4); H(0,0) = 1.0; H(1,1) = 1.0;
    Matrix Q = Matrix::identity(4); for(int i = 0; i < 4; i++) Q(i,i) = 0.1;
    Matrix R = Matrix::identity(2); R(0,0) = R(1,1) = 0.25;
    Matrix P0 = Matrix::identity(4); for(int i = 0; i < 4; i++) P0(i,i) = 1000.0;
    kf.setF(F); kf.setH(H); kf.setQ(Q); kf.setR(R); kf.setInitialState(Matrix(4,1), P0);

    normal_distribution<> noise(0.0, 0.5);
    const int steps = 1000000;
    vector<Matrix> measurements;
    measurements.reserve(1000);
    for(int i = 0; i < 1000; i++){
        Matrix z(2, 1);
        z(0,0) = i * dt * 10 + noise(gen);
        z(1,0) = i * dt * 5 + noise(gen);
        measurements.push_back(z);
    }
    auto t0 = Clock::now();
    for(int i = 0; i < steps; i++){
        kf.predict();
        kf.update(measurements[i % 1000]);
    }
    auto t1 = Clock::now();
    sink = kf.getState()(0,0);
    double total_us = chrono::duration<double, micro>(t1 - t0).count();
    cout << steps << " steps: " << setprecision(3) << total_us / steps
         << " us per predict+update (" << setprecision(0)
         << steps / (total_us / 1e6) << " steps/s)\n";
    return 0;
}
