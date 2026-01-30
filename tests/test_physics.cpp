#include<iostream>
#include<iomanip>
#include "../src/physics/Particle.hpp"
using namespace std;

int main(){
    cout << "Starting Monte Carlo Simulation" << endl;

    Particle p(0, 0, 10, 10);
    cout << "Time  | X    | Y    | X Measured | Y Measured" << endl;
    cout << "--------------------------------------" << endl;

    double dt = 0.1;
    for(int i = 0; i < 10; i++){
        p.update(dt);
        Matrix real = p.getTrueState();
        Matrix meas = p.measure();

        cout << fixed << setprecision(2)
            << (i+1)*dt << "s |"
            << real(0,0) << "  |" << real(1,0) << "  |"
            << meas(0,0) << "        |" << meas(1,0) << endl;
    }
    return 0;
}
