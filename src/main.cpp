#include<iostream>
#include<iomanip>
#include<fstream>
using namespace std;
#include "../src/core/Matrix.hpp"
#include "../src/physics/Particle.hpp"
#include "../src/physics/KalmanFilter.hpp"

int main(){
    cout << "=== Kalman Filter Tracking ===" << endl;
    double dt = 0.1;

    Particle particle(0,0,10,5);
    KalmanFilter kft(4,2);

    Matrix F(4,4);
    F = Matrix::identity(4);
    F(0,2) = dt;
    F(1,3) = dt;
    kft.setF(F);

    Matrix H(2,4);
    H(0,0) = 1.0;
    H(1,1) = 1.0;
    kft.setH(H);

    // Small process noise: the simulated particle moves at constant velocity.
    Matrix Q = Matrix::identity(4);
    for(int i=0; i < 4; i++){
        Q(i,i) = 1e-3;
    }
    kft.setQ(Q);

    // Measurement noise covariance matches the simulated sensor (variance = std^2).
    const double r = Particle::MEASUREMENT_NOISE_STD * Particle::MEASUREMENT_NOISE_STD;
    Matrix R(2,2);
    R(0,0) = r;
    R(1,1) = r;
    kft.setR(R);

    Matrix x0(4,1);
    Matrix P0 = Matrix::identity(4);
    for(int i =0; i < 4; ++i){
        P0(i,i) = 1000.0;
    }
    kft.setInitialState(x0, P0);


    std::ofstream file("results.csv");
    file << "time,x,measured_x,estimated_x(kalman)\n";

    for(int i = 0; i < 50; ++i){
        particle.update(dt);
        Matrix real = particle.getTrueState();
        Matrix meas = particle.measure();

        kft.predict();
        kft.update(meas);

        Matrix estimated = kft.getState();

        file << (i+1)*dt << "," << real(0,0) 
        << "," << meas(0,0) << "," << estimated(0,0) << "\n";
    }
    file.close();
    cout << "Data saved at 'results.csv'" << endl;

    return 0;
}