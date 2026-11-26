
#include <cmath>
#include <cstdio>
#include "../src/core/Matrix.hpp"
#include "../src/physics/Particle.hpp"
#include "../src/physics/KalmanFilter.hpp"

KalmanFilter makeFilter(double dt) {
    KalmanFilter kf(4, 2);
    Matrix F = Matrix::indentity(4);
    F(0,2) = dt; F(1,3) = dt;
    kf.setF(F);

    Matrix H(2, 4);
    H(0,0) = 1.0; H(1,1) = 1.0;
    kf.setH(H);

    Matrix Q = Matrix::indentity(4);
    for (int i = 0; i < 4; ++i) Q(i,i) = 0.1;
    kf.setQ(Q);

    Matrix R(2, 2);
    R(0,0) = 2.0; R(1,1) = 2.0;
    kf.setR(R);

    Matrix P0 = Matrix::indentity(4);
    for (int i = 0; i < 4; ++i) P0(i,i) = 1000.0;
    kf.setInitialState(Matrix(4, 1), P0);
    return kf;
}

int main() {
    const int RUNS = 1000;
    const int STEPS = 50;
    const double dt = 0.1;

    double se_meas = 0.0, se_kf = 0.0;
    long count = 0;

    for (int r = 0; r < RUNS; ++r) {
        Particle particle(0, 0, 10, 5);
        KalmanFilter kf = makeFilter(dt);

        for (int i = 0; i < STEPS; ++i) {
            particle.update(dt);
            Matrix truth = particle.getTrueState();
            Matrix z = particle.measure();

            kf.predict();
            kf.update(z);
            Matrix est = kf.getState();

            for (int c = 0; c < 2; ++c) {
                se_meas += std::pow(z(c,0)   - truth(c,0), 2);
                se_kf   += std::pow(est(c,0) - truth(c,0), 2);
            }
            ++count;
        }
    }

    double rmse_meas = std::sqrt(se_meas / count);
    double rmse_kf   = std::sqrt(se_kf / count);
    std::printf("Monte Carlo: %d runs x %d steps\n", RUNS, STEPS);
    std::printf("RMSE raw sensor : %.4f m\n", rmse_meas);
    std::printf("RMSE Kalman     : %.4f m\n", rmse_kf);
    std::printf("Reduction       : %.1f%%\n", 100.0 * (1.0 - rmse_kf / rmse_meas));
    return 0;
}