#include<iostream>
#include<cassert>
#include"../src/core/Matrix.hpp"
using namespace std;

int main(){
    cout << "Testing: " << endl;

    Matrix A(2, 3);
    A(0,0) = 1; A(0, 1) = 2; A(0,2) = 3;
    A(1,0) = 4; A(1,1) = 5; A(1,2) = 6;

    Matrix B(3,2);
    B(0,0) = 7; B(0,1) = 8;
    B(1,0) = 9; B(1,1) = 1;
    B(2,0) = 2; B(2,1) = 3;

    Matrix C = A * B;
    cout << "Matrix A:" << endl;
    A.print();
    cout << "\nMatrix B:" << endl;
    B.print();

    if(C(0,0) == 31.0){
        cout << "\n[SUCESS]" << endl;
    } else {
        cout << "\n[ERROR]" << endl;
    }
    return 0;
}