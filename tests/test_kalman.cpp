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
    F = Matrix::indentity(4);
    F(0,2) = dt;
    F(1,3) = dt;
    kft.setF(F);

    Matrix H(2,4);
    H(0,0) = 1.0;
    H(1,1) = 1.0;
    kft.setH(H);

    Matrix Q(4,4);
    Q = Matrix::indentity(4);
    for(int i=0; i < 4; i++){
        Q(i,i) = 0.1;
    }
    kft.setQ(Q);

    Matrix R(2,2);
    R(0,0) = 2.0;
    R(1,1) = 2.0;
    kft.setR(R);

    Matrix x0(4,1);
    Matrix P0 = Matrix::indentity(4);
    for(int i =0; i < 4; ++i){
        P0(i,i) = 1000.0;
    }
    kft.setInitialState(x0, P0);

    cout << "Time |   X   | X Measured | X Estimated (Kalman)" << endl;
    cout << "--------------------------------------------" << endl;

    for(int i = 0; i < 15; ++i){
        particle.update(dt);
        Matrix real = particle.getTrueState();
        Matrix meas = particle.measure();

        kft.predict();
        kft.update(meas);

        Matrix estimated = kft.getState();

        cout << fixed << setprecision(2) << (i+1)*dt << "s| " << real(0,0) 
        << "  | " << meas(0,0) << "       | " << estimated(0,0) << endl;
    }
    return 0;
}