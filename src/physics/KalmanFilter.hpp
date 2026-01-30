#ifndef KALMAN_FILTER_HPP
#define KALMAN_FILTER_HPP

#include "../core/Matrix.hpp"

class KalmanFilter {
private:
    Matrix x;
    Matrix P;
    Matrix F;
    Matrix Q;
    Matrix H;
    Matrix R;
    Matrix I;
public:
    KalmanFilter(int state_dim, int mean_dim);
    void predict();
    void update(const Matrix& z_measured);

    Matrix getState() const{
        return x;
    }
    Matrix getCovariance() const{
        return P;
    }

    void setF(const Matrix& F_in){
        F = F_in;
    }
    void setQ(const Matrix& Q_in){
        Q = Q_in;
    }
    void setH(const Matrix& H_in){
        H = H_in;
    }
    void setR(const Matrix& R_in){
        R = R_in;
    }
    void setInitialState(const Matrix& x0, const Matrix& P0){
        x = x0;
        P = P0;
    }
};

#endif