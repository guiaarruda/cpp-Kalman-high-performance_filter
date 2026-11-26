#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include "../src/core/Matrix.hpp"

static bool near(double a, double b, double eps = 1e-9) { return std::fabs(a - b) < eps; }

static bool equal(const Matrix& A, const Matrix& B, double eps = 1e-9) {
    if (A.getRows() != B.getRows() || A.getColumns() != B.getColumns()) return false;
    for (size_t i = 0; i < A.getRows(); ++i)
        for (size_t j = 0; j < A.getColumns(); ++j)
            if (!near(A(i, j), B(i, j), eps)) return false;
    return true;
}

void test_multiplication() {
    Matrix A(2, 3);
    A(0,0)=1; A(0,1)=2; A(0,2)=3;
    A(1,0)=4; A(1,1)=5; A(1,2)=6;
    Matrix B(3, 2);
    B(0,0)=7; B(0,1)=8;
    B(1,0)=9; B(1,1)=1;
    B(2,0)=2; B(2,1)=3;

    Matrix C = A * B;

    assert(C.getRows() == 2 && C.getColumns() == 2);
    assert(near(C(0,0), 31)); assert(near(C(0,1), 19));
    assert(near(C(1,0), 85)); assert(near(C(1,1), 55));
    assert(equal(C, A.multiplyNaive(B)));   // otimizada == original

    bool threw = false;
    try { A * A; } catch (const std::invalid_argument&) { threw = true; }
    assert(threw);
}

void test_add_sub_transpose() {
    Matrix A(2, 3);
    A(0,0)=1; A(0,1)=2; A(0,2)=3;
    A(1,0)=4; A(1,1)=5; A(1,2)=6;
    Matrix B(2, 3, 1.0);

    Matrix S = A + B;
    assert(near(S(0,0), 2) && near(S(1,2), 7));
    assert(equal(S - B, A));

    Matrix T = A.transpose();
    assert(T.getRows() == 3 && T.getColumns() == 2);
    assert(near(T(0,1), 4) && near(T(2,0), 3));
    assert(equal(T.transpose(), A));
}

void test_identity() {
    Matrix I = Matrix::indentity(3);
    for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 3; ++j)
            assert(near(I(i,j), i == j ? 1.0 : 0.0));
}

void test_inverse() {
    Matrix A(2, 2);
    A(0,0)=4; A(0,1)=7;
    A(1,0)=2; A(1,1)=6;
    Matrix Ai = A.inverse();
    assert(near(Ai(0,0), 0.6) && near(Ai(0,1), -0.7));
    assert(near(Ai(1,0), -0.2) && near(Ai(1,1), 0.4));
    assert(equal(A * Ai, Matrix::indentity(2)));

    Matrix P(3, 3);
    P(0,0)=0; P(0,1)=2; P(0,2)=1;
    P(1,0)=1; P(1,1)=1; P(1,2)=0;
    P(2,0)=3; P(2,1)=0; P(2,2)=1;
    assert(equal(P * P.inverse(), Matrix::indentity(3)));

    Matrix Sg(2, 2);
    Sg(0,0)=1; Sg(0,1)=2;
    Sg(1,0)=2; Sg(1,1)=4;
    bool threw = false;
    try { Sg.inverse(); } catch (const std::runtime_error&) { threw = true; }
    assert(threw);
}

void test_out_of_range() {
    Matrix A(2, 2);
    bool threw = false;
    try { A(2, 0); } catch (const std::out_of_range&) { threw = true; }
    assert(threw);
}

int main() {
    test_multiplication();
    test_add_sub_transpose();
    test_identity();
    test_inverse();
    test_out_of_range();
    std::cout << "All matrix tests passed" << std::endl;
    return 0;
}