// Monte Carlo evaluation of the Kalman filter: runs many independent simulations and
// compares the position RMSE of the raw measurements with the RMSE of the filter estimate.
#include<iostream>
#include<iomanip>
#include<cassert>
#include<cmath>
#include "../src/core/Matrix.hpp"
#include "../src/physics/Particle.hpp"
#include "../src/physics/KalmanFilter.hpp"
using namespace std;

static KalmanFilter makeFilter(double dt, double q, double r){
    KalmanFilter kf(4, 2);
    Matrix F = Matrix::identity(4);
    F(0,2) = dt;
    F(1,3) = dt;
    kf.setF(F);

    Matrix H(2, 4);
    H(0,0) = 1.0;
    H(1,1) = 1.0;
    kf.setH(H);

    Matrix Q = Matrix::identity(4);
    for(int i = 0; i < 4; i++) Q(i,i) = q;
    kf.setQ(Q);

    Matrix R(2, 2);
    R(0,0) = r;
    R(1,1) = r;
    kf.setR(R);

    Matrix P0 = Matrix::identity(4);
    for(int i = 0; i < 4; i++) P0(i,i) = 1000.0;
    kf.setInitialState(Matrix(4, 1), P0);
    return kf;
}

// Returns {measurement RMSE, filter RMSE} on the x position, ignoring the warm-up steps.
static pair<double,double> evaluate(double q, double r, int trials, int steps, int warmup){
    const double dt = 0.1;
    double sum_meas = 0.0, sum_est = 0.0;
    long count = 0;
    for(int t = 0; t < trials; t++){
        Particle particle(0, 0, 10, 5);
        KalmanFilter kf = makeFilter(dt, q, r);
        for(int i = 0; i < steps; i++){
            particle.update(dt);
            Matrix real = particle.getTrueState();
            Matrix meas = particle.measure();
            kf.predict();
            kf.update(meas);
            if(i < warmup) continue;
            double em = meas(0,0) - real(0,0);
            double ek = kf.getState()(0,0) - real(0,0);
            sum_meas += em * em;
            sum_est += ek * ek;
            count++;
        }
    }
    return {sqrt(sum_meas / count), sqrt(sum_est / count)};
}

int main(){
    const int trials = 1000, steps = 50, warmup = 10;
    const double sigma = Particle::MEASUREMENT_NOISE_STD;

    cout << fixed << setprecision(3);
    cout << "Monte Carlo: " << trials << " runs x " << steps
         << " steps (first " << warmup << " steps ignored)\n";

    // Original hand-picked tuning vs. tuning that matches the simulation:
    // R = sensor variance, and a small Q because the particle moves at constant velocity.
    struct Tuning { const char* name; double q; double r; };
    const Tuning tunings[] = {
        {"original (Q=0.1, R=2.0)", 0.1, 2.0},
        {"matched  (Q=1e-3, R=sigma^2)", 1e-3, sigma * sigma},
    };
    for(const Tuning& t : tunings){
        auto [rmse_meas, rmse_est] = evaluate(t.q, t.r, trials, steps, warmup);
        double reduction = 100.0 * (1.0 - rmse_est / rmse_meas);
        cout << t.name << " | RMSE measurement = " << rmse_meas
             << " | RMSE Kalman = " << rmse_est
             << " | error reduction = " << setprecision(1) << reduction << "%\n"
             << setprecision(3);
        assert(rmse_est < rmse_meas);
    }
    return 0;
}
