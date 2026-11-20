#ifndef PARTICLE_HPP
#define PARTICLE_HPP
#include "../core/Matrix.hpp"

class Particle{
public:
    // Standard deviation of the Gaussian noise added to each position measurement.
    static constexpr double MEASUREMENT_NOISE_STD = 0.5;
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
