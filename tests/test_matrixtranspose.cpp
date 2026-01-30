#include<iostream>
#include<cassert>
#include "../src/core/Matrix.hpp"
using namespace std;  
int main(){
    cout << "Transpose Test" << endl;
    Matrix A(2,3);
    A(0,0) = 1; A(0,1) = 2; A(0,2) = 3;
    A(1,0) = 4; A(1,1) = 5; A(1,2) = 6;

    cout << "Original Matrix(2x3):" << endl;
    A.print();

    Matrix T = A.transpose();
    cout << "\nTranspose (3x2):" << endl;
    T.print();

    Matrix B(2, 3, 1.0);
    Matrix C = A + B;
    cout << "\nSum:" << endl;
    C.print();

    if(C(0,0) == 2.0 && T(0,1) == 4.0){
        cout << "\n[SUCESS]" << endl;
    } else{
        cout <<"\n[ERROR]" << endl;
    }
    return 0;
}