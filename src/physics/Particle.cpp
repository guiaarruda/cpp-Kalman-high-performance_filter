#include<random>
#include "Particle.hpp"
using namespace std;

static random_device rd;
static mt19937 gen(rd());
static normal_distribution<> noise_dist(0.0, 0.5);

Particle::Particle(double x, double y, double vx, double vy)
    : real_state(4, 1){
        real_state(0,0) = x;
        real_state(1,0) = y;
        real_state(2,0) = vx;
        real_state(3,0) = vy;
    }
void Particle::update(double dt){
    real_state(0,0) += real_state(2,0) * dt;
    real_state(1,0) += real_state(3,0) * dt;
}
Matrix Particle::measure(){
    Matrix measurement(2,1);

    measurement(0,0) = real_state(0,0) + noise_dist(gen);
    measurement(1,0) = real_state(1,0) + noise_dist(gen);

    return measurement;
}