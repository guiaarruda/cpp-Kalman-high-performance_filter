#include<iostream>
#include"../src/core/Matrix.hpp"
using namespace std;

int main(){
    cout << "Testing..." << endl;

    Matrix A(2,2);
    A(0,0) = 4; A(0,1) = 7;
    A(1,0) = 2; A(1,1) = 6;

    cout << "Matrix A:" << endl;
    A.print();

    try{
        Matrix A_inverse = A.inverse();
        cout << "Inverse Matrix:" << endl;
        A_inverse.print();

        cout << "\nA * A_inverse (must be identity):" << endl;
        Matrix I = A * A_inverse;
        I.print();
    } catch (const exception& e){
        cout << "Error: " << e.what() << endl;
    }
    return 0;
}