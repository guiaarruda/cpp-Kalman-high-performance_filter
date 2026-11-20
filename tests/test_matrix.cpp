// Unit tests for the Matrix class. Exits with a non-zero code on the first failure.
#include<iostream>
#include<cassert>
#include<cmath>
#include<random>
#include "../src/core/Matrix.hpp"
using namespace std;

static bool near(double a, double b, double tol = 1e-9){
    return std::fabs(a - b) < tol;
}

static bool isIdentity(const Matrix& M, double tol = 1e-9){
    for(size_t i = 0; i < M.getRows(); i++)
        for(size_t j = 0; j < M.getColumns(); j++)
            if(!near(M(i,j), i == j ? 1.0 : 0.0, tol)) return false;
    return true;
}

int main(){
    // Construction and element access
    Matrix mat(3, 3, 0.0);
    mat(0,2) = 5.5;
    assert(mat.getRows() == 3 && mat.getColumns() == 3);
    assert(mat(0,2) == 5.5 && mat(1,1) == 0.0);

    // Out-of-range access throws
    bool threw = false;
    try { mat(3,0); } catch(const std::out_of_range&){ threw = true; }
    assert(threw);

    // Multiplication (2x3 * 3x2)
    Matrix A(2, 3);
    A(0,0) = 1; A(0,1) = 2; A(0,2) = 3;
    A(1,0) = 4; A(1,1) = 5; A(1,2) = 6;
    Matrix B(3, 2);
    B(0,0) = 7; B(0,1) = 8;
    B(1,0) = 9; B(1,1) = 1;
    B(2,0) = 2; B(2,1) = 3;
    Matrix C = A * B;
    assert(C.getRows() == 2 && C.getColumns() == 2);
    assert(C(0,0) == 31 && C(0,1) == 19 && C(1,0) == 85 && C(1,1) == 55);

    // Incompatible dimensions throw
    threw = false;
    try { A * A; } catch(const std::invalid_argument&){ threw = true; }
    assert(threw);

    // Transpose, addition, subtraction
    Matrix T = A.transpose();
    assert(T.getRows() == 3 && T.getColumns() == 2);
    assert(T(0,1) == 4 && T(2,0) == 3);
    Matrix S = A + Matrix(2, 3, 1.0);
    assert(S(0,0) == 2 && S(1,2) == 7);
    Matrix D = S - A;
    assert(D(1,1) == 1);

    // Inverse of a 2x2 matrix
    Matrix M(2, 2);
    M(0,0) = 4; M(0,1) = 7;
    M(1,0) = 2; M(1,1) = 6;
    Matrix Minv = M.inverse();
    assert(near(Minv(0,0), 0.6) && near(Minv(0,1), -0.7));
    assert(near(Minv(1,0), -0.2) && near(Minv(1,1), 0.4));
    assert(isIdentity(M * Minv));

    // Inverse needs pivoting (zero on the diagonal)
    Matrix P(2, 2);
    P(0,0) = 0; P(0,1) = 1;
    P(1,0) = 1; P(1,1) = 0;
    assert(isIdentity(P * P.inverse()));

    // Singular matrix throws
    Matrix Sing(2, 2, 1.0);
    threw = false;
    try { Sing.inverse(); } catch(const std::runtime_error&){ threw = true; }
    assert(threw);

    // Random 50x50 matrix: A * A^-1 must be the identity
    mt19937 gen(42);
    uniform_real_distribution<> dist(-1.0, 1.0);
    Matrix R(50, 50);
    for(size_t i = 0; i < 50; i++)
        for(size_t j = 0; j < 50; j++)
            R(i,j) = dist(gen) + (i == j ? 50.0 : 0.0);
    assert(isIdentity(R * R.inverse(), 1e-9));

    cout << "All Matrix tests passed." << endl;
    return 0;
}
