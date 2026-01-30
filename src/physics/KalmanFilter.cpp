#include "KalmanFilter.hpp"

KalmanFilter::KalmanFilter(int state_dim, int meas_dim)
    : x(state_dim, 1), P(state_dim, state_dim), F(state_dim, state_dim), Q(state_dim, state_dim),
    H(meas_dim, state_dim), R(meas_dim, meas_dim), I(Matrix::indentity(state_dim)){}

void KalmanFilter::predict(){
    x = F * x;
    Matrix FT = F.transpose();
    P = (F * P) * FT + Q;
}

void KalmanFilter::update(const Matrix& z){
    Matrix y = z - (H*x);

    Matrix HT = H.transpose();
    Matrix S = (H * P) * HT + R;

    Matrix K  = (P*HT) * S.inverse();

    x = x + (K*y);
    P = (I - (K *H)) * P;
}