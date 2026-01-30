#ifndef MATRIX_HPP
#define MATRIX_HPP
#include<vector>
#include<iostream>
#include<stdexcept>

class Matrix {
private:
    std::vector<double> data;
    size_t rows;
    size_t columns;
public:
    Matrix(size_t r, size_t c, double initial_value = 0.0);

    double& operator()(size_t row, size_t column);
    const double& operator()(size_t row, size_t column) const;

    size_t getRows() const{
        return rows;
    }
    size_t getColumns() const{
        return columns;
    }   
    
    Matrix operator*(const Matrix& other) const;
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;

    Matrix transpose() const;

    Matrix inverse() const;
    static Matrix indentity(size_t size);
    void print() const;
};

#endif