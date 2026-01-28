#include<iostream>
using namespace std;
#include "../src/core/Matrix.hpp"

int main(){
    cout << "Creating a 3x3 matrix..." << endl;
    Matrix mat(3, 3, 0.0);
    mat(0,0) = 1.0;
    mat(1,1) = 1.0;
    mat(2,2) = 1.0;

    mat(0,2) = 5.5;

    cout << "Matrix Content:" << endl;

    mat.print();
    cout << "It worked!!" << endl;

    return 0;
}