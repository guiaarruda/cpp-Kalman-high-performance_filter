#ifndef PARTICLE_HPP
#define PARTICLE_HPP
#include "../core/Matrix.hpp"

class Particle{
private:
    Matrix real_state;
public:
    Particle(double x, double y, double vx, double vy);

    void update(double dt);

    Matrix measure();

    Matrix getTrueState() const {
        return real_state;
    }
};
#endif
